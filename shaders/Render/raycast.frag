#version 450

in FragmentData
{
	vec3 rayStart;
};

layout(location = 0) out vec4 outColor;

//layout(location = 3) uniform isampler3D distanceField;

layout(r32i, location = 3) uniform restrict iimage3D distanceField;

layout(location = 2) uniform vec3 Eye;

const float SampleStep = 0.005;

vec3 shadeFrom(vec3 pos, vec3 norm)
{
	vec3 Kd = vec3(0, 0.807, 0.819);
	vec3 Ks = vec3(0, 0.907, 0.619);
	vec3 color = vec3(1, 1, 1);
	float sE = 8.0;

	vec3 to_light = vec3(0, 1, 0);

	vec3 diffuse = clamp(dot(to_light, norm), 0, 1) * color * Kd;

	vec3 to_eye = normalize(Eye - pos);
	vec3 specular = pow(clamp(dot(reflect(-to_light, norm), to_eye), 0, 1), sE) * Ks * color;

	return diffuse + specular;
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

		density = float(imageLoad(distanceField, ivec3(rayPos * 64))) / 10000;

		color.rgb = vec3(0, 0.807, 0.819);
		color.a = exp(-1.0 * (density * density - 1.0)) * SampleStep * 0.1 * 50.0;
		fragColor.rgb = fragColor.rgb * (1.0 - color.a) + color.rgb * color.a;
	}

	if(length(fragColor) < 0.1)
		discard;

	//float val =
	outColor = vec4(fragColor, 1);
}
