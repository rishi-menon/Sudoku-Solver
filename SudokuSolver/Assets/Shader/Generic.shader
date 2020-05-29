#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float textureId;
layout(location = 3) in vec2 textureCoord;

out vec4 v_col;
out float v_textureId;
out vec2 v_textureCoord;

uniform mat4 u_Mvp;

void main()
{
   v_col = color;
   v_textureId = textureId;
   v_textureCoord = textureCoord;

   gl_Position = u_Mvp * position;
}

#shader fragment
#version 330 core


in vec4 v_col;

in float v_textureId;
in vec2 v_textureCoord;

out vec4 color;

uniform sampler2D u_textureIds[2];
void main()
{
   int id = int(v_textureId);
   vec4 texCol = texture (u_textureIds[id], v_textureCoord);
   color = vec4(v_col.x * texCol.x, v_col.y * texCol.y, v_col.z * texCol.z, v_col.a * texCol.a);
}