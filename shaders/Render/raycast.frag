#version 450

in FragmentData
{
	vec3 rayStart;
};

layout(location = 0) out vec4 outColor;

layout(location = 0) uniform sampler3D distanceText;

layout(location = 1) uniform vec3 Eye;

struct Plain
{
	vec3 normal;
	vec3 point;
};

const Plain borders[6] =
{
	{vec3(-1,  0,  0), vec3( 0,  0,  0)},
	{vec3( 1,  0,  0), vec3( 1,  0,  0)},

	{vec3( 0, -1,  0), vec3( 0,  0,  0)},
	{vec3( 0,  1,  0), vec3( 0,  1,  0)},

	{vec3( 0,  0, -1), vec3( 0,  0,  0)},
	{vec3( 0,  0,  1), vec3( 0,  0,  1)}
};

const float SampleStep = 0.005;

vec3 getNorm(vec3 pos)
{
	 vec3 norm;
	 norm.x = texture(distanceText, pos + vec3(1, 0, 0) / 64.0).r - texture(distanceText, pos - vec3(1, 0, 0) / 64.0).r;
	 norm.y = texture(distanceText, pos + vec3(0, 1, 0) / 64.0).r - texture(distanceText, pos - vec3(0, 1, 0) / 64.0).r;
	 norm.z = texture(distanceText, pos + vec3(0, 0, 1) / 64.0).r - texture(distanceText, pos - vec3(0, 0, 1) / 64.0).r;

	 return normalize(norm);
}

vec3 shade(vec3 pos, vec3 ray)
{
	const vec3 Kd = vec3(0, 0.807, 0.819);
	const vec3 Ks = vec3(0, 0.907, 0.98);
	const vec3 color = vec3(0.7, 0.7, 0.7);
	const float sE = 8.0;
	const vec3 to_light = normalize(vec3(0.3, 1, 0.3));

	vec3 norm = getNorm(pos);

	vec3 diffuse = 0.7 * min(abs(dot(to_light, norm)), 1) * color * Kd;

	vec3 specular = pow(clamp(dot(reflect(-to_light, norm), -ray), 0, 1), sE) * Ks * color;

	return 0.01 * Kd + diffuse + specular;
}

bool inCube(vec3 pos)
{
	return
		all(lessThan(pos, vec3(1, 1, 1))) &&
		all(greaterThanEqual(pos, vec3(0, 0, 0)));
}

bool inCube(vec2 pos)
{
	return
		all(lessThan(pos, vec2(1, 1))) &&
		all(greaterThanEqual(pos, vec2(0, 0)));
}

float hitPlane(uint i, vec3 rayPos, vec3 rayDir)
{
	return max(0,
		dot(borders[i].point - rayPos, borders[i].normal) /
		dot(rayDir, borders[i].normal)
	);
}

void hitX(vec3 rayPos, vec3 rayDir, inout float minAlpha)
{
	float alpha = hitPlane(0, rayPos, rayDir);
	if(inCube((rayPos + alpha * rayDir).yz))
		minAlpha = min(alpha, minAlpha);

	alpha = hitPlane(1, rayPos, rayDir);
	if(inCube((rayPos + alpha * rayDir).yz))
		minAlpha = min(alpha, minAlpha);
}

void hitY(vec3 rayPos, vec3 rayDir, inout float minAlpha)
{
	float alpha = hitPlane(2, rayPos, rayDir);
	if(inCube((rayPos + alpha * rayDir).xz))
		minAlpha = min(alpha, minAlpha);

	alpha = hitPlane(3, rayPos, rayDir);
	if(inCube((rayPos + alpha * rayDir).xz))
		minAlpha = min(alpha, minAlpha);
}

void hitZ(vec3 rayPos, vec3 rayDir, inout float minAlpha)
{
	float alpha = hitPlane(4, rayPos, rayDir);
	if(inCube((rayPos + alpha * rayDir).xy))
		minAlpha = min(alpha, minAlpha);

	alpha = hitPlane(5, rayPos, rayDir);
	if(inCube((rayPos + alpha * rayDir).xy))
		minAlpha = min(alpha, minAlpha);
}

void hitBorders(inout vec3 rayPos, vec3 rayDir)
{
	float minAlpha = 10;
	hitX(rayPos, rayDir, minAlpha);
	hitY(rayPos, rayDir, minAlpha);
	hitZ(rayPos, rayDir, minAlpha);

	rayPos = rayPos + minAlpha * rayDir;
}

void main()
{
	const vec3 rayDir = normalize(rayStart - Eye);
	vec3 rayPos = rayStart;
	hitBorders(rayPos, rayDir);

	vec4 fragColor = vec4(0, 0, 0, 0);
	vec4 color;
	float dist = 0;
	for(int i = 0; i < 400; ++i)
	{
		rayPos += SampleStep * rayDir;

		if(!inCube(rayPos))
			break;

		dist = texture(distanceText, rayPos).r;
		if(dist < 0.01 || dist > 0.03)
			continue;

		float x = dist - 0.02;
		color.a = exp(-160000 * x * x) * SampleStep * 2;

		color.rgb = shade(rayPos, rayDir);

		fragColor.rgb = fragColor.rgb * fragColor.a + color.rgb * (1 - fragColor.a);
		fragColor.a += (1 - fragColor.a) * color.a;

		if(fragColor.a >= 0.99)
			break;

	}

	outColor = fragColor;
}
