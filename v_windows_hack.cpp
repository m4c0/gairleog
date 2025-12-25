module v;
import :tools;

// Contains template instantiations that explodes clang on Windows for some
// weird reason

v::app_stuff::vtx_attrs_t v::app_stuff::vertex_attributes() {
  return clay::buffer<sprite>::vertex_attributes(
      &v::sprite::pos,
      &v::sprite::scale,
      &v::sprite::mult,
      &v::sprite::grid_pos,
      &v::sprite::grid_size,
      &v::sprite::rotation,
      &v::sprite::id);
}

