//#version 450

//layout(binding = 1) uniform sampler2D texSampler;

//layout(location = 0) in vec3 fragColor;
//layout(location = 1) in vec2 fragTexCoord;

//layout(location = 0) out vec4 outColor;

//void main() {
//	//outColor = vec4(fragTexCoord, 0.0, 1.0);
//	outColor = texture(texSampler, fragTexCoord);
//	//outColor = texture(texSampler, fragTexCoord * 4.0);
//	//outColor = vec4(fragColor * texture(texSampler, fragTexCoord * 2.0).rgb, 1.0);
//}

// need hard/sharp edges
#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    // Calculate the grayscale value from the texture coordinate
    float grayValue = dot(fragTexCoord, vec2(0.299, 0.587));

    // Set the grayscale value as the RGB color
    outColor = vec4(grayValue, grayValue, grayValue, 1.0);
}
