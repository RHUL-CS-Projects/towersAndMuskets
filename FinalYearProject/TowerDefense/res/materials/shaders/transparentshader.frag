
uniform sampler2D myTexture;
uniform double alpha;

void main (void)
{
    vec4 col = texture2D(myTexture, vec2(gl_TexCoord[0]));
    col *= alpha;
    gl_FragColor = col;
}
