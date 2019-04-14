#version 450

in FragmentData
{
	vec3 rayStart;
};

layout(location = 0) out vec4 outColor;

layout(location = 0) uniform sampler3D distanceText;

layout(location = 1) uniform vec3 Eye;

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
	const float sE = 16.0;
	const vec3 to_light = normalize(vec3(0.3, 1, 0.3));

	vec3 norm = getNorm(pos);

	vec3 diffuse = 0.7 * clamp(dot(to_light, norm), 0, 1) * color * Kd;

	vec3 specular = pow(clamp(dot(reflect(-to_light, norm), -ray), 0, 1), sE) * Ks * color;

	return 0.1 * Kd + diffuse + specular;
}

void main()
{
	const vec3 rayDir = normalize(rayStart - Eye);
	vec3 rayPos = rayStart;

	vec4 fragColor = vec4(0, 0, 0, 0);
	vec4 color;
	float density = 0;
	for(int i = 0; i < 700; ++i)
	{
		rayPos += SampleStep * rayDir;

		if(any(greaterThan(rayPos, vec3(1))) || any(lessThan(rayPos, vec3(0))))
			break;

		density = texture(distanceText, rayPos).r;

		//if(density < 0.0 || density > 2)
		//	continue;

		float x = density - 1.5;
		color.a = exp(-90.0 * x * x) * SampleStep * 50.0;
		//if(color.a > 0.9)
		//{
			color.rgb = shade(rayPos, rayDir);//vec3(0, 0.807, 0.819);

			fragColor.rgb = fragColor.rgb * fragColor.a + color.rgb * (1 - fragColor.a);
			fragColor.a += (1 - fragColor.a) * color.a;

			if(fragColor.a >= 1.0)
				break;
		//}

	}

	//if(length(fragColor) < 0.1)
	//	discard;

	//float val =
	outColor = fragColor;
}
