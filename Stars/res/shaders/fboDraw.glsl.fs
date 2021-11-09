#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D renderedTexture;
uniform sampler2D bloomTexture;
uniform int bloomLODs;

void main()
{ 
    vec4 col = vec4(0);
    for(int i = 1; i < bloomLODs; i++)
    {
         col += textureLod(bloomTexture, texCoords, i);
    }
    float div = col.length / 2;
    col.x /= div;
    col.y /= div;
    col.z /= div;
    vec4 renderCol = texture(renderedTexture, texCoords);

    vec4 testCol = textureLod(bloomTexture, texCoords, 0);
    if((testCol.x > 0 || testCol.y > 0 || testCol.z > 0))
        fragColor = renderCol - vec4(6, 6, 6, 0);
    else
        fragColor = renderCol + col;
}