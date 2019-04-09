#version 450

in FragmentData
{
	vec3 rayStart;
};

layout(location = 0) out vec4 outColor;

layout(location = 3) uniform sampler3D distanceText;

layout(location = 2) uniform vec3 Eye;

const float SampleStep = 0.005;

vec3 getNorm(vec3 pos)
{
	 vec3 norm;
	 norm.x = texture(distanceText, pos + vec3(1, 0, 0) / 64).r - texture(distanceText, pos - vec3(1, 0, 0) / 64).r;
	 norm.y = texture(distanceText, pos + vec3(0, 1, 0) / 64).r - texture(distanceText, pos - vec3(0, 1, 0) / 64).r;
	 norm.z = texture(distanceText, pos + vec3(0, 0, 1) / 64).r - texture(distanceText, pos - vec3(0, 0, 1) / 64).r;

	return normalize(norm);
}

vec3 shade(vec3 pos, vec3 ray)
{
	const vec3 Kd = vec3(0, 0.807, 0.819);
	const vec3 Ks = vec3(0, 0.907, 0.619);
	const vec3 color = vec3(0.7, 0.7, 0.7);
	const float sE = 6.0;
	const vec3 to_light = normalize(vec3(0.3, 1, 0.3));

	vec3 norm = getNorm(pos);

	vec3 diffuse = clamp(dot(to_light, norm), 0, 1) * color * Kd;

	vec3 specular = pow(clamp(dot(reflect(-to_light, norm), -ray), 0, 1), sE) * Ks * color;

	return 0.1 * Kd + diffuse + specular;
}

void main()
{
	const vec3 rayDir = normalize(rayStart - Eye);
	vec3 rayPos = rayStart;

	vec3 fragColor = vec3(0);
	vec4 color;
	float density = 0;
	for(int i = 0; i < 700; ++i)
	{
		rayPos += SampleStep * rayDir;

		if(any(greaterThan(rayPos, vec3(1))) || any(lessThan(rayPos, vec3(0))))
			break;

		density = texture(distanceText, rayPos).r;

		color.a = exp(-1.0 * (density * density + 2.0)) * /* SampleStep */ 2 * 50.0;
		if(color.a > 0.9)
		{
			color.rgb = shade(rayPos, rayDir);//vec3(0, 0.807, 0.819);

			fragColor.rgb = fragColor.rgb * (1.0 - color.a) + color.rgb * color.a;
			break;
		}

	}

	//if(length(fragColor) < 0.1)
	//	discard;

	//float val =
	outColor = vec4(fragColor, 1);
}
