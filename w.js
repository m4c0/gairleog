!function() {
  // Created with: spirv-cross gairleog.frag.spv --es --version 300
  const frag_shader = `#version 300 es
    precision mediump float;
    precision highp int;
    
    uniform highp sampler2D tex;
    
    layout(location = 0) out highp vec4 colour;
    in highp vec2 f_uv;
    
    void main()
    {
        colour = texelFetch(tex, ivec2(f_uv * 16.0), 0);
    }
  `;

  // Created with: spirv-cross gairleog.vert.spv --es --version 300
  const vert_shader = `#version 300 es
    
    struct upc
    {
        vec2 grid_pos;
        vec2 grid_size;
    };
    
    uniform upc _18;
    
    layout(location = 0) in vec2 v_pos;
    layout(location = 1) in vec2 pos;
    out vec2 f_uv;
    layout(location = 2) in vec2 uv;
    
    void main()
    {
        vec2 p = ((v_pos + pos) - _18.grid_pos) / _18.grid_size;
        gl_Position = vec4(p, 0.0, 1.0);
        f_uv = uv + v_pos;
    }
  `;

  const canvas = document.getElementById("casein-canvas");
  const gl = canvas.getContext("webgl2");
  const ext = gl.getExtension("ANGLE_instanced_arrays");

  const prog = gl.createProgram();

  const vert = gl.createShader(gl.VERTEX_SHADER);
  gl.shaderSource(vert, vert_shader);
  gl.compileShader(vert);
  if (!gl.getShaderParameter(vert, gl.COMPILE_STATUS)) {
    console.error(gl.getShaderInfoLog(vert));
  }
  gl.attachShader(prog, vert);

  const frag = gl.createShader(gl.FRAGMENT_SHADER);
  gl.shaderSource(frag, frag_shader);
  gl.compileShader(frag);
  if (!gl.getShaderParameter(frag, gl.COMPILE_STATUS)) {
    console.error(gl.getShaderInfoLog(frag));
  }
  gl.attachShader(prog, frag);

  gl.linkProgram(prog);
  if (!gl.getProgramParameter(prog, gl.LINK_STATUS)) {
    console.error(gl.getProgramInfoLog(prog));
  }

  gl.useProgram(prog);
}();
