uniform sampler2D fbo_texture;
varying vec2 f_texcoord;
 
void main(void) {
  gl_FragColor = texture2D(fbo_texture, f_texcoord);
}
