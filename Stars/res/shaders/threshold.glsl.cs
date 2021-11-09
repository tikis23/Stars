#version 430
layout(local_size_x = 16, local_size_y = 8) in;
layout(rgba16f, binding = 0) readonly uniform image2D img_input;
layout(rgba16f, binding = 1) writeonly uniform image2D img_output;


void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    vec4 color = imageLoad(img_input, pixelCoord);
    float threshold = 4;
    if (color.x > threshold || color.y > threshold || color.z > threshold)
        imageStore(img_output, pixelCoord, color - vec4(6, 6, 6, 0));
    else
        imageStore(img_output, pixelCoord, vec4(0, 0, 0, 0));
}