# Not yet functional


LIBS = -L./GLFW/lib-linux-x86_64/

INCLUDES = -I./GLFW/include/ -I./glad/include/

CPP=g++
CPP_OPTS=-Wall -Wextra --std=c++11 -O2 -g

# GLM_HEADERS = glm/common.hpp glm/detail/_features.hpp glm/detail/_fixes.hpp glm/detail/_noise.hpp glm/detail/_swizzle.hpp glm/detail/_swizzle_func.hpp glm/detail/_vectorize.hpp glm/detail/func_common.hpp glm/detail/func_exponential.hpp glm/detail/func_geometric.hpp glm/detail/func_integer.hpp glm/detail/func_matrix.hpp glm/detail/func_packing.hpp glm/detail/func_trigonometric.hpp glm/detail/func_vector_relational.hpp glm/detail/precision.hpp glm/detail/setup.hpp\
#	glm/detail/type_float.hpp glm/detail/type_gentype.hpp glm/detail/type_half.hpp glm/detail/type_int.hpp glm/detail/type_mat.hpp glm/detail/type_mat2x2.hpp glm/detail/type_mat2x3.hpp glm/detail/type_mat2x4.hpp glm/detail/type_mat3x2.hpp glm/detail/type_mat3x3.hpp glm/detail/type_mat3x4.hpp glm/detail/type_mat4x2.hpp glm/detail/type_mat4x3.hpp glm/detail/type_mat4x4.hpp glm/detail/type_vec.hpp glm/detail/type_vec1.hpp glm/detail/type_vec2.hpp glm/detail/type_vec3.hpp\
#	glm/detail/type_vec4.hpp glm/exponential.hpp glm/ext.hpp glm/fwd.hpp glm/geometric.hpp glm/glm.hpp glm/gtc/bitfield.hpp glm/gtc/color_space.hpp glm/gtc/constants.hpp glm/gtc/epsilon.hpp glm/gtc/functions.hpp glm/gtc/integer.hpp glm/gtc/matrix_access.hpp glm/gtc/matrix_integer.hpp glm/gtc/matrix_inverse.hpp glm/gtc/matrix_transform.hpp glm/gtc/noise.hpp glm/gtc/packing.hpp glm/gtc/quaternion.hpp glm/gtc/random.hpp glm/gtc/reciprocal.hpp glm/gtc/round.hpp\
#	glm/gtc/type_aligned.hpp glm/gtc/type_precision.hpp glm/gtc/type_ptr.hpp glm/gtc/ulp.hpp glm/gtc/vec1.hpp glm/gtx/associated_min_max.hpp glm/gtx/bit.hpp glm/gtx/closest_point.hpp glm/gtx/color_space.hpp glm/gtx/color_space_YCoCg.hpp glm/gtx/common.hpp glm/gtx/compatibility.hpp glm/gtx/component_wise.hpp glm/gtx/dual_quaternion.hpp glm/gtx/euler_angles.hpp glm/gtx/extend.hpp glm/gtx/extended_min_max.hpp glm/gtx/fast_exponential.hpp glm/gtx/fast_square_root.hpp\
#	glm/gtx/hash.hpp glm/gtx/fast_trigonometry.hpp glm/gtx/gradient_paint.hpp glm/gtx/handed_coordinate_space.hpp glm/gtx/integer.hpp glm/gtx/intersect.hpp glm/gtx/io.hpp glm/gtx/log_base.hpp glm/gtx/matrix_cross_product.hpp glm/gtx/matrix_decompose.hpp glm/gtx/matrix_interpolation.hpp glm/gtx/matrix_major_storage.hpp glm/gtx/matrix_operation.hpp glm/gtx/matrix_query.hpp glm/gtx/matrix_transform_2d.hpp glm/gtx/mixed_product.hpp glm/gtx/norm.hpp glm/gtx/normal.hpp\
#	glm/gtx/normalize_dot.hpp glm/gtx/number_precision.hpp glm/gtx/optimum_pow.hpp glm/gtx/orthonormalize.hpp glm/gtx/perpendicular.hpp glm/gtx/polar_coordinates.hpp glm/gtx/projection.hpp glm/gtx/quaternion.hpp glm/gtx/range.hpp glm/gtx/raw_data.hpp glm/gtx/rotate_normalized_axis.hpp glm/gtx/rotate_vector.hpp glm/gtx/scalar_multiplication.hpp glm/gtx/scalar_relational.hpp glm/gtx/simd_mat4.hpp glm/gtx/simd_quat.hpp glm/gtx/simd_vec4.hpp glm/gtx/spline.hpp\
#	glm/gtx/std_based_type.hpp glm/gtx/string_cast.hpp glm/gtx/transform.hpp glm/gtx/transform2.hpp glm/gtx/type_aligned.hpp glm/gtx/type_trait.hpp glm/gtx/vector_angle.hpp glm/gtx/vector_query.hpp glm/gtx/wrap.hpp glm/integer.hpp glm/mat2x2.hpp glm/mat2x3.hpp glm/mat2x4.hpp glm/mat3x2.hpp glm/mat3x3.hpp glm/mat3x4.hpp glm/mat4x2.hpp glm/mat4x3.hpp glm/mat4x4.hpp glm/matrix.hpp glm/packing.hpp glm/trigonometric.hpp glm/vec2.hpp glm/vec3.hpp glm/vec4.hpp glm/vector_relational.hpp 

STB_HEADERS = stb/stb_image.h stb/stb_image_resize.h stb/stb_image_write.h

# GLFW_HEADERS = GLFW/include/GLFW/glfw3.h GLFW/include/GLFW/glfw3native.h

# GLAD_HEADERS = glad/include/glad/glad.h

CNS_HEADERS = Camera.h CNS.h DTGlobal.h Keys.h Loop.h Perspective.h RenderData.h Texture.h

HEADERS = $(STB_HEADERS) $(CNS_HEADERS)

OBJECTS = Main.o Camera.o CNS.o Keys.o Loop.o Perspective.o RenderData.o Texture.o

all: $(OBJECTS) 
	$(CPP) $(CPP_OPTS) $^ -o CarbonNanoSweeper $(LIBS)

Main.o: Main.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

Camera.o: Camera.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

CNS.o: CNS.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

Keys.o: Keys.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

Loop.o: Loop.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

Perspective.o: Perspective.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

RenderData.o: RenderData.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)

Texture.o: Texture.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) $< $(HEADERS) -o $@ $(INCLUDES)