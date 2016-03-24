
uniform sampler2D myTexture;
uniform float alpha;
uniform bool useIDCol;
uniform vec4 idCol;

void main (void)
{
    vec4 col = texture2D(myTexture, vec2(gl_TexCoord[0]));
    col *= alpha;
    if (useIDCol)
		col = idCol;
    gl_FragColor = col;
}
