#version 430
layout(local_size_x = 4, local_size_y = 2) in;
layout(rgba16f, binding = 0) uniform image2D img_input;

uniform ivec2 dir;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 result = imageLoad(img_input, (pixelCoord)) * weight[0];
    //ivec2 imgSize = imageSize(img_input);
    for (int i = 1; i < 5; i++)
    { 
        result += imageLoad(img_input, pixelCoord + i * dir) * weight[i];
        result += imageLoad(img_input, pixelCoord - i * dir) * weight[i];
    }
    imageStore(img_input, pixelCoord, result);
}