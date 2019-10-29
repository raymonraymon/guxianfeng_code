/*!
*      \file main.cpp
*      \brief Main program for Slit Mapping/Riemann Mapping/Harmonic Mapping
*	   \author David Gu
*      \date Document 10/07/2010
*
*		The code performs the following tasks
*
*       1. Compute the harmonic maps for simply connected domain with a single boundary component to the unit disk.
*       2. Compute the Riemann mapping ( conformal mapping) from a simply connected domain with a single boundary component to the unit disk.
*	    3. Compute the slit mapping from a multiply connected domain to planar disk with concentric circular arcs,
*          one boundary component is mapped to the center circular hole, the orther boundary components are mapped to 
*          concentric circular arcs.
*/

/*!	\mainpage   Harmonic Mapping, Riemann Mapping, Slit Mapping, Ricci Flow
 *	
 *	\section intro_sec Introduction
 *	
 *  Mesh parameterization is an important operation in digital geometry process. Suppose \f$ S \f$ is a surface
 *  embedded in the three dimensional Euclidean space \f$ R^3\f$. \f$\phi: S\to D\f$ is a mapping, which
 *  maps the surface onto the unit planar disk \f$ D\f$, the mapping is a diffeomorphism. The \f$\phi\f$ is called a 
 *  surface parameterization. 
 *  
 *
 * \section data_structure_sec HalfEdge Data Structure
 *	All the computationsa are conducted on discrete surfaces. The smooth surface is sampled and triangualted,
 *  we use piecewise Euclidean surface to approximate the original surfaces. Then the triangle mesh is represented
 *  using the so-called HalfEdge data structure, the details for triangle meshes can be found <A HREF=halfedge.html><B>here</B></A>.
 *
 *	\section algorithm_sec Computational Algorithm
 * <HR>
 *  \subsection harmonic_map_sub_sec Harmonic Mapping Algorithm
  *  There are many ways to compute surface parameterization. The simplest way is to compute harmonic maps. Intutively, we 
 *  imagine the surface is made of rubber sheet, deformations will introduce stretching energy, but no bending energy. A harmonic map
 *  optimizes the stretching energy with the given boundary condition. It has many advantages
 *  <OL>
 *  <LI> If the boundary is mapped to a convex planar polygon, and the boundary mapping is a homeomorphism, then the mapping is a diffeomorphism.
 *  <LI> Computing harmonic map is equivalent to solve an elliptic Partial Differential Equation on the surface, numerically stable.
 *  <LI> Position constraints can be incoporated in the mapping directly.
 *  </OL>
 *  The computation details for harmonic mapping are explained <A HREF=harmonic_mapping_steps.html> <B>here</B></A>. The lecture note on harmonic map can be found 
 *  <A HREF="lecture_Harmonic_Maps.pdf"> <B>here</B></A>.
 * 
 * <table border="0" cellspacing="0">
* <tr>
* <td><IMG SRC="HarmonicMap/alex_icon.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="HarmonicMap/alex_disk_icon.PNG" alt="Disk Image" width="320" height="320"></td>
* <td><IMG SRC="HarmonicMap/alex_circle_icon.PNG" alt="Circles Texture Mapping"  width="320" height="320"></td>
* <td><IMG SRC="HarmonicMap/alex_checker_icon.PNG" alt="Circles Texture Mapping"  width="320" height="320"></td>
* </tr>
* <tr>
* <td><IMG SRC="HarmonicMap/sophie_icon.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="HarmonicMap/sophie_disk_icon.PNG" alt="Disk Image" width="320" height="320"></td>
* <td><IMG SRC="HarmonicMap/sophie_circle_icon.PNG" alt="Circles Texture Mapping"  width="320" height="320"></td>
* <td><IMG SRC="HarmonicMap/sophie_checker_icon.PNG" alt="Circles Texture Mapping"  width="320" height="320"></td>
* </tr>
* </table>
*  
* 
*<HR>
*  \subsection riemann_sub_sec Riemann Mapping Algorithm
*
*	Suppose \f$S\f$ is a surface with a Riemannian metric \f$ \mathbf{g}\f$, a mapping \f$ f: S\to D\f$ maps
*   the surface onto the planar unit disk \f$ D\f$. If the pull back metric induced by 
*   \f[
*	f^{*}\mathbf{g}_{E^2} = e^{2\lambda} \mathbf{g}
*	\f]
*	then \f$f\f$ is a Riemann mapping.
*
*   The existence and uniqueness of conformal mapping is stated in the Riemann mapping theorem: Suppose \f$S\f$ 
*   is a metric surface, which is of genus zero with a single boundary component (a topological disk). Then there exist conformal mappings, which map S to the planar unit disk, such that the pull back metric induced by such a mapping on S differs from the original metric by a scalar function. Furthermore, two such kind of mappings differ by a Mobius transformation of the planar unit disk. 
*   The computation details for Riemann mapping are explained <A HREF=Riemann_mapping_steps.html> <B>here</B></A>.
*
*   <table border="0" cellspacing="0">
* <tr>
* <td><IMG SRC="face/riemann_mapper_logo_small.png" width="320" height="320"></td>
* </tr>
* </table>
*
*<HR>
* \subsection slit_map_sub_sec Slit Mapping Algorithm
*
* 	Suppose \f$S\f$ is a genus 0 surface with multiple boundary components. \f$ f: S \to D\f$ is a conformal mapping, which maps
*   the exterior boundary component and one interior boundary component to concentric circles, and all other boundary components are
*   mapped to concentric circular slits. The computation details for slit mapping are explained <A HREF=Slit_map_steps.html> <B>here</B></A>. The lecture note on slit map can be found 
*  <A HREF="lecture_Slit_Circle_Map.pdf"> <B>here</B></A>.
* <table border="0" cellspacing="0">
* <tr>
* <td><IMG SRC="slitmaps/face_holes.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="slitmaps/face_holes_checker.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="slitmaps/face_01.circ_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="slitmaps/face_02.circ_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* <tr>
* <td><IMG SRC="slitmaps/face_03.circ_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="slitmaps/face_12.circ_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="slitmaps/face_13.circ_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="slitmaps/face_23.circ_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*<HR>
* \subsection Ricci_flow_sub_sec Euclidean Ricci Flow Algorithm
*
*		Compute Euclidean Ricci Flow using Tangential Circle Packing Metric
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="RicciFlow/sophie.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RicciFlow/sophie_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RicciFlow/sophie_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
*  <tr>
* <td><IMG SRC="RicciFlow/kitten_high_res.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RicciFlow/kitten.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RicciFlow/kitten_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*
*  The Euclidean tangential Ricci flow command is <P>
*  <B> RicciFlow -ricci_flow sophie.remesh.m sophie.uv.m</B><P>
*  <B> RicciFlow -ricci_flow kitten.m kitten.open.m kitten.uv.m</B>
*  The Euclidean inversive distance Ricci flow command is <P>
*  <B> RicciFlow -inverse_ricci_flow sophie.remesh.m sophie.uv.m</B><P>
*  <B> RicciFlow -inverse_ricci_flow kitten.m kitten.open.m kitten.uv.m</B>
*  The Euclidean Yamabe flow command is <P>
*  <B> RicciFlow -yamabe_flow kitten.m kitten.open.m kitten.uv.m</B>
*
*<HR>
* \subsection hyper_Ricci_flow_sub_sec Hyperbolic Ricci Flow Algorithm
*
*		Compute Hyperbolic Ricci Flow using Tangential Circle Packing Metric
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="RicciFlow/genus2.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RicciFlow/genus2_open.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="RicciFlow/genus2_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RicciFlow/genus2_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
*  The Hyperbolic tangential Ricci flow command is <P>
*  <B> RicciFlow -hyper_ricci_flow genus2.m genus2.open.m genus2.uv.m</B><P>
*  The Hyperbolic inversive distance Ricci flow command is <P>
*  <B> RicciFlow -hyper_inverse_ricci_flow genus2.m genus2.open.m genus2.uv.m</B><P>
*  The Hyperbolic Yamabe flow command is <P>
*  <B> RicciFlow -hyper_yamabe_flow genus2.m genus2.open.m genus2.uv.m</B><P>
*
*<HR>
* \subsection curvature_flow_extremal_length_sub_sec Curvature Flow for Extremal Length Algorithms
*
*		Compute Extremal Length using Curvature Flow
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="ExtremalLengthCF/alex.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="ExtremalLengthCF/alex_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="ExtremalLengthCF/alex_check.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="ExtremalLengthCF/alex_circle.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="ExtremalLengthCF/sophie.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="ExtremalLengthCF/sophie_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="ExtremalLengthCF/sophie_check.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="ExtremalLengthCF/sophie_circle.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
*  Preprocess: label four corners at four vertices on the boundary.<P>
*
*  The command for computing Extremal length using tangential Ricci flow <P>
*  <B> RicciFlow -tangent_ricci_extremal_length sophie.m sophie.uv.m</B><P>
*  <B> RicciFlow -tangent_ricci_extremal_length alex.m alex.uv.m</B><P>
*  The command for computing Extremal length using inversive distance Ricci flow <P>
*  <B> RicciFlow -idrf_extremal_length sophie.m sophie.uv.m</B><P>
*  <B> RicciFlow -idrf_extremal_length alex.m alex.uv.m</B><P>
*  The command for computing Extremal length using Yamabe flow <P>
*  <B> RicciFlow -yamabe_extremal_length sophie.m sophie.uv.m</B><P>
*  <B> RicciFlow -yamabe_extremal_length alex.m alex.uv.m</B><P>
*
*<HR>
* \subsection curvature_flow_riemann_mapping_sub_sec Curvature Flow for Riemann Mapping Algorithms
*
*		Compute Riemann Mapping using Curvature Flow
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="RiemannMappingCF/alex.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RiemannMappingCF/alex_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="RiemannMappingCF/alex_check.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RiemannMappingCF/alex_circle.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="RiemannMappingCF/sophie.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RiemannMappingCF/sophie_pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="RiemannMappingCF/sophie_check.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="RiemannMappingCF/sophie_circle.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
*  Preprocess:
*  RiemannMapper.exe -puncture alex.remesh.m alex.m<P>
*  RiemannMapper.exe -cut alex.m alex.cut<P>
*  RiemannMapper.exe -slice alex.cut.m alex.open.m<P>
*
*  The command for computing Riemann Mapping using tangential Ricci flow <P>
*  <B> RicciFlow -ricci_riemann_map sophie.m sophie.uv.m</B><P>
*  <B> RicciFlow -ricci_riemann_map alex.m alex.uv.m</B><P>
*  The command for computing Riemann Mapping using inversive distance Ricci flow <P>
*  <B> RicciFlow -idrf_riemann_map sophie.m sophie.uv.m</B><P>
*  <B> RicciFlow -idrf_riemann_map alex.m alex.uv.m</B><P>
*  The command for computing Riemann Mapping using Yamabe flow <P>
*  <B> RicciFlow -yamabe_riemann_map sophie.m sophie.uv.m</B><P>
*  <B> RicciFlow -yamabe_riemann_map alex.m alex.uv.m</B><P>
*
*<HR>
* \subsection curvature_flow_poly_annulus_sub_sec Curvature Flow for Poly Annulus
*
*		Compute Circle domain mapping for poly annulus using Curvature Flow
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="PolyAnnulusCF/alex_hole.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="PolyAnnulusCF/alex_hole_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="PolyAnnulusCF/alex_hole_check.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="PolyAnnulusCF/alex_hole_circle.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="PolyAnnulusCF/sophie_hole.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="PolyAnnulusCF/sophie_hole.pos.PNG" alt="Input Mesh" width="320" height="320"></td>
* <tr>
* <td><IMG SRC="PolyAnnulusCF/sophie_hole_check.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="PolyAnnulusCF/sophie_hole_circle.PNG" alt="Input Mesh" width="320" height="320"></td>
* </table>
*
*  Preprocess: remove holes from alex.remesh.m
*  RiemannMapper.exe -cut alex_hole.m alex_hole <P>	  
*  RiemannMapper.exe -slice alex_hole.cut.m alex_hole.open.m <P>
*
*  The command for computing circular domain mapping for poly annulus using tangential Ricci flow <P>
*  <B> RicciFlow -ricci_poly_annulus alex_hole.m alex_hole.open.m alex.uv.m</B><P>
*  The command for computing circular domain mapping for poly annulus using inversive distance Ricci flow <P>
*  <B> RicciFlow -idrf_poly_annulus alex_hole.m alex_hole.open.m alex.uv.m</B><P>
*  The command for computing circular domain mapping for poly annulus using Yamabe flow <P>
*  <B> RicciFlow -yamabe_poly_annulus alex_hole.m alex_hole.open.m alex.uv.m</B><P>
*
* <HR>
*
* \subsection Delaunay_sub_sec Mesh Generation based on Delaunay Triangulation Algorithm
*
*   Mesh generation using Delaunay Refinement (Ruppert's method)
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="delaunay/simple.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="delaunay/square_cut.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*
*  The input is a .poly file, PLSG. The output is a meshed file. The command is <P>
*  <B> delaunay.exe -poly poly_file_name mesh_file_name</B>
*<HR>
* \subsection remesh_sub_sec Remeshing based on Delaunay Triangulation Algorithm
*
*   Remesh using Delaunay Refinement (Ruppert's method)
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="DelaunayRemesh/sophie_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="DelaunayRemesh/sophie_remesh.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
*  <tr>
* <td><IMG SRC="DelaunayRemesh/alex_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="DelaunayRemesh/alex_remesh.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*
*  The input is a .m file. The output is a refined meshed file. The command is <P>
*  <B> DelaunayRemesh.exe -remesh input_mesh_file_name refined_mesh_file_name</B>
*<HR>
* \subsection refine_sub_sec Mesh Refinement based on Delaunay Triangulation Algorithm
*
*   Mesh generation using Delaunay Refinement (Ruppert's method)
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="DelaunayRefine/sophie_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="DelaunayRefine/sophie_refine.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
*  <tr>
* <td><IMG SRC="DelaunayRefine/alex_uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="DelaunayRefine/alex_refine.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*
*  The input is a .m file. The output is a refined meshed file. The command is <P>
*  <B> DelaunayRefine.exe -refine input_mesh_file_name refined_mesh_file_name</B>
*<HR>
*  \subsection mobius_sub_sec Mobius transformation
*
*  Compute and visualize Mobius transformation. The input is a mesh with rgb and uv coordinates. Command <P>
*  <B> Mobius.exe sophie.uv.m <B>
*  <P>
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="Viewer/Mobius_1.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/Mobius_2.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/Mobius_3.png" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*<HR>
*\subsect 3D triangle mesh viewer
*
*  Visualize a textured mesh. The input is a mesh with uv coordinates and texture bmp image. Command <P>
*  <B> ViewerConverter.exe sophie.uv.obj sophie.bmp <B><P>
*  <B> ViewerConverter.exe sophie.uv.m sophie.bmp <B><P>
*  <P>
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="Viewer/Sophie_geom.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/Sophie.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/Sophie_texture.png" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* <tr>
* <td><IMG SRC="Viewer/alex_geom.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/alex.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/alex_texture.png" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* <tr>
* <td><IMG SRC="Viewer/susan_geom.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/susan.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Viewer/susan_texture.png" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*<HR>
*\subsect CutGraph
*
*  Compute the Cut Graph of a high genus closed mesh. Command <P>
*  <B> RiemannMapper.exe -cut_graph kitten.nf20k.m kitten.nf20k.cut.m <B><P>
*  <P>
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="CutGraph/kitten.nf20k.png" alt="Input Mesh" width="360" height="360"></td>
* <td><IMG SRC="CutGraph/kitten.nf20k.cut.png" alt="Input Mesh" width="360" height="360"></td>
* </tr>
* </table>

*<HR>
*\subsect Homology Basis
*
*  Compute the first homology group generators of a high genus closed mesh.<P> 
*  Command <P>
*  <B> RiemannMapper.exe -homology eight.m base <B><P>
*  <B> RiemannMapper.exe -slice base_0.m base_0.open.m <B><P>
*  <B> RiemannMapper.exe -slice base_1.m base_1.open.m <B><P>
*  <B> RiemannMapper.exe -slice base_2.m base_2.open.m <B><P>
*  <B> RiemannMapper.exe -slice base_3.m base_3.open.m <B><P>
*  <P>
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="Homology/base_0.open.png" alt="Input Mesh" width="360" height="360"></td>
* <td><IMG SRC="Homology/base_1.open.png" alt="Input Mesh" width="360" height="360"></td>
* <td><IMG SRC="Homology/base_2.open.png" alt="Input Mesh" width="360" height="360"></td>
* <td><IMG SRC="Homology/base_3.open.png" alt="Input Mesh" width="360" height="360"></td>
* </tr>
* </table>
*
* <HR>
* \subsect Centroidal Voronoi Tessellation
*
* Compute the Delaunay triangulation based on centroidal voronoi tessellation
*	
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="CVTDelaunay/input.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="CVTDelaunay/output.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*
* <HR>
* \subsect Variational Delaunay triangulation
*
* Delaunatry triangulation optimized by maximizing the total volumes of the Hyperbolic Ideal tetrahedron 
*
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="VariationalDelaunay/input.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="VariationalDelaunay/output.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* <HR>
* \subsect Harmonic 1-forms
*
* Compute harmonic 1-form group basis
*
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="HarmonicForm/harmonic_0.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="HarmonicForm/harmonic_1.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="HarmonicForm/harmonic_2.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="HarmonicForm/harmonic_3.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
*  
* <HR>
* \subsect Holomorphic 1-forms
*
* Compute holomorphic 1-form group basis
*
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="HolomorphicForm/holomorphic_0.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="HolomorphicForm/holomorphic_1.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="HolomorphicForm/holomorphic_2.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="HolomorphicForm/holomorphic_3.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="HolomorphicForm/combine_1.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="HolomorphicForm/combine_2.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
* <HR>
* \subsect Tight fundamental group generators
*
*  Directory: Demos\\Dijkstra_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="Dijkstra/eight_1.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="Dijkstra/eight_2.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="Dijkstra/eight_3.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="Dijkstra/eight_4.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* <OL>
* <LI> Compute a Riemannian metric with negative curvature <P>
*      RicciFlow -graph_ricci_flow %mesh%.m %mesh%_0.m
* <LI> Compute the shortest essential loop through a given vertex<P>
*		RiemannMapper.exe -shortest_loop %mesh%_0.m %mesh%_0.m %root% loop_0.txt
* <LI> Slice the mesh open along the loop<P>
*		RiemannMapper.exe -surgery %mesh%_0.m loop_0.txt %mesh%_1.m
* <LI> Compute the shortest essential loop connecting two corresponding vertices on the boundary <P>
*	RiemannMapper.exe -essential_loop %mesh%_%previous%.m  %mesh%_%current%.m loop_%current%.txt
* <LI> Slice the mesh along the loop<P>
*		RiemannMapper.exe -surgery %mesh%_%current%.m  loop_%current%.txt %mesh%_%next%.m
* <LI> Repeat step 4 and 5.
* </OL>
*
* <HR>
* \subsect Koebe's iteration method
*
*  Directory: Demos\\koebe_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="Koebe/sophie.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="koebe/step_7.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the conformal mapping, which maps a multiply connected surface onto a circle domain on the plane by
* koebe's iteration method.
*
* <HR>
* \subsect Extremal Length using holomorphic 1-form method
*
*  Directory: Demos\\ExtremalLength_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="ExtremalLength/sophie_uv.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="ExtremalLength/sophie.pos.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="ExtremalLength/alex_uv.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="ExtremalLength/face.pos.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the conformal mapping, which maps a multiply connected surface onto a circle domain on the plane by
* koebe's iteration method.
*
* <HR>
* \subsect Genus one surface conformal mapping using holomorphic 1-form method
*
*  Directory: Demos\\genus_one_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="genus_one/kitten.nf20k.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="genus_one/kitten_uv.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="genus_one/rocker.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="genus_one/rocker_arm_uv.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the conformal mapping for genus one surface using holomorphic one form method.
* <HR>
* \subsect Maximal hyperbolic circle packing 
*
*  Directory: Demos\\Ricciflow_hyperbolic_Riemann_Mapping_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="Ricciflow_hyperbolic_Riemann_Mapping/1.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="Ricciflow_hyperbolic_Riemann_Mapping/2.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="Ricciflow_hyperbolic_Riemann_Mapping/3.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="Ricciflow_hyperbolic_Riemann_Mapping/4.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the maximal circle packing using hyperbolic Ricci flow by setting the boundary radii to infinity.
* This method can be test if an infinite graph is hyperbolic or parabolic.
*
* <HR>
* \subsect Covering Space 
*
*  Directory: Demos\\CoveringSpace_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="CoveringSpace/kitten.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="CoveringSpace/eight.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the Deck transformation group generators, and cover finite portion of the universal covering space.
*
* <HR>
* \subsect Diagonal Ratio Mapping, compute conformal mapping using Diagonal Ratio 
*
*  Directory: Demos\\DiagonalRatioMapping_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="DiagonalRatioMapping/Sophie_checker.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatioMapping/Sophie_circle.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatioMapping/Sophie_pos.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="DiagonalRatioMapping/alex_checker.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatioMapping/alex_circle.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatioMapping/alex_pos.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the Conformal Mapping using Diagonal Ratio.
*
* <HR>
* \subsect Diagonal Ratio, compute holomorphic 1-forms using Diagonal Ratio 
*
*  Directory: Demos\\Diagonal_Ratio_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="DiagonalRatio/kitten.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatio/kitten_uv.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="DiagonalRatio/holomorphic_0.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatio/holomorphic_1.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="DiagonalRatio/holomorphic_2.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatio/holomorphic_3.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
*  <tr>
* <td><IMG SRC="DiagonalRatio/holo_0.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="DiagonalRatio/holo_1.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the Conformal Mapping using Diagonal Ratio.
* <HR>
* \subsect Quasi-Conformal Riemann Mapping, compute Quasi-Conformal Riemann Mapping, using holomorphic 1-form method 
*
*  Directory: Demos\\QC_RiemannMapping_Form_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="QC_RiemannMapping_Form/sophie_checker.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="QC_RiemannMapping_Form/sophie_circle.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*  
* Compute the Quasi-Conformal Mapping using holomorphic 1-form
* <HR>
* \subsect Quasi-Conformal Riemann Mapping, compute Quasi-Conformal Riemann Mapping, using Ricci Flow method 
*
*  Directory: Demos\\QC_RiemannMapping_test\\
*  
* <table border="0" cellspacing="0">
*  <tr>
* <td><IMG SRC="QC_RiemannMapping/sophie_pos.PNG" alt="Input Mesh" width="600" height="360"></td>
* <td><IMG SRC="QC_RiemannMapping/sophie_circle.PNG" alt="Input Mesh" width="600" height="360"></td>
* </tr>
* </table>
*
* <HR>
*  Compute the principle direction. The input is a mesh. Command <P>
*  <B> PrincipalCurvature.exe bunnyuv.m <B><P>
*  <P>
*  Then press 'p'
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="PrincipleCurvature/bunny.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="PrincipleCurvature/camel.png" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*  
* <HR>
*  TetMesh Viewer. Command <P>
*  <B> TetMeshViewer.exe bimba_xyz.tet <B><P>
*   Directory: Demos\\TetMeshViewer_test\\<P>
*  <P>
*  Then press 'g'
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="TMeshViewer/bimba_front.png" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="TMeshViewer/bimba_back.png" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*  
* <HR>
*  Hodge Decomposition. Command <P>
*  <B> RiemannMapper.exe eight.m eight.du.m <B><P>
*   Directory: Demos\\Hodge_Decomposition_Test\\<P>
*  <P>
*  The program randomly generate a 1-form, \f$ \omega \f$, then its Hodge decomposition is  
\f[
\omega=df+\delta g + h,
\f]
\f$f\f$ is stored in 
*  the vertex, \f$ g \f$ is installed on the faces, \f$h\f$ is stored on each edge.
* 
* <table border="0" cellspacing="0"> 
* <tr>
* <td><IMG SRC="Hodge_Decomposition/eight.PNG" alt="Input Mesh" width="320" height="320"></td>
* <td><IMG SRC="Hodge_Decomposition/eight.uv.PNG" alt="Input Mesh" width="320" height="320"></td>
* </tr>
* </table>
*  
* Compute the Quasi-Conformal Mapping using Ricci flow.
*/





#include "API.h"

using namespace MeshLib;

/*!	\brief Helper function to remind the users of the usage of the commands	
 *
 */

void help(char * exe )
{
	printf("Usage:\n");
	printf("%s -puncture original_mesh puncture_mesh\n", exe);
	printf("%s -cut puncture_mesh prefix_of_cut_mesh\n", exe);
	printf("%s -slice cut_mesh open_mesh\n", exe);
	printf("%s -exact_form puncture_mesh prefix_exact_form\n", exe);
	printf("%s -holomorphic_form exact_form_mesh_0 ... exact_form_mesh_n closed_form_mesh_0 ... closed_form_mesh_n\n", exe);
	printf("%s -slit_map holomorphic_form_0 holomorphic_form_1 ... holomorphic_form_n c_1 c_2 output_holomorphic_form\n", exe );
	printf("%s -integration holomorpic_form_k.m open_mesh uv_mesh\n", exe);
	printf("%s -polar_map puncture_mesh uv_mesh circular_uv_mesh\n", exe);
	printf("%s -fill_hole circular_uv_mesh filled_mesh\n", exe );
	printf("%s --------------------------------------------------------------------------------------------------------\n", exe );
	printf("%s -hole2poly input_mesh hole.poly\n", exe );
	printf("%s -off2mesh  off_file output_mesh\n", exe );
	printf("%s -merge     base_mesh patch_mesh union_mesh\n", exe );
	printf("%s --------------------------------------------------------------------------------------------------------\n", exe );
	printf("%s -harmonic_map  input_mesh output_mesh_with_uv\n", exe );
	//updated functionalities
	printf("%s -spherical_harmonic_map  input_mesh output_mesh\n", exe );
	//compute the cut graph
	printf("%s -cut_graph  input_mesh output_mesh\n", exe );
	//compute homology
	printf("%s -homology  input_mesh output_mesh_prefix\n", exe );
	//compute harmonic 1-form
	//printf("%s -harmonic_one_form  input_closed_mesh mesh_sliced_open output_1_form_mesh output_uv_mesh\n", exe );
	//compute closed 1-form
	printf("%s -cohomology_one_form  input_closed_mesh mesh_sliced_open output_1_form_mesh output_uv_mesh\n", exe );
	//compute closed 1-form
	printf("%s -cohomology_one_form_domain  input_domain_mesh mesh_sliced_open output_1_form_mesh output_uv_mesh\n", exe );
	//compute harmonic 1-form for extremal length
	printf("%s -extremal_length  input_topo_disk_mesh prefix_output_mesh\n", exe );
	//linear combination of holomorphic 1-forms
	printf("%s -linear_combination  output_mesh holomorphic_form_1 lambda_1 holomorphic_form_2 lambda_2 ... holomorphic_form_n lambda_n\n", exe );
	//shortest non-separating loop through the root vertex
	printf("%s -shortest_loop mesh_file mesh_file root_id loop_file\n");
	//shortest non-separting loop connecting two corresponding boundary vertices
	printf("%s -essential_loop previous_mesh_file current_mesh_file loop_file\n");
	//slice the mesh along the loops
	printf("%s -surgery mesh_file loop_file_0 loop_file_1 ... loop_file_n open_mesh_file\n");
	//double covering
	printf("%s -double_covering mesh_with_boundaries closed_symmetric_mesh\n");
	//fill center hole
	printf("%s -fill_center_hole mesh_with_boundaries_uv mesh_with_center_hole_filled\n");
	//remove segment
	printf("%s -remove_segment mesh_with_segment_id segment_id mesh_with_segment_removed\n");
};







/*!	\brief main function to call all the functionalities
 * 
 */

int main( int argc, char * argv[] )
{


	if( argc < 3 )
	{
		help( argv[0] );
		return 0;
	}



	/*!	compute the slit map for topological multi-hole anuli to a planar annulus with concentric circular slits
	*
	*/
	if( strcmp( argv[1], "-slit_map" ) == 0 )
	{
		_slit_map( argc, argv );
		return 0;
	}

	/*! compute the basis for all the exact harmonic forms on a surface
	 *
	 */
	if( strcmp( argv[1], "-exact_form" ) == 0 )
	{
		_exact_form( argv[2], argv[3] );
		return 0;
	}
	/*!	compute the basis of all holomorphic 1-forms on a surface
	 *
	 */

    if( strcmp( argv[1], "-holomorphic_form" ) == 0 )
	{
		_holomorphic_form( argc, argv );
		return 0;
	}

  /*!	Integrate a holomorphic 1-form on a simply connected surface
   *
   */

	if( strcmp( argv[1], "-integration" ) == 0 )
	{
		_integration( argv[2], argv[3], argv[4] );
		return 0;
	}

	/*! Compute the exponential map
	 *
	 */
	if( strcmp( argv[1], "-polar_map" ) == 0 )
	{
		//closed mesh, open mesh, expential image
		_polar_map( argv[2], argv[3], argv[4] );
		return 0;
	}

	/*!	diffuse the closed one form basis to a harmonic one form
	 *
	 */

	if( strcmp( argv[1], "-diffuse" ) == 0 )
	{
		_diffuse( argv[2], argv[3], argv[4], argv[5] );
		return 0;
	}


/**************************************************************************************************************************************
*
*	Topological Algorithms
*
*
***************************************************************************************************************************************/


	


	/*! Slice the mesh open along the sharp edges
	 *
	 */
	if( strcmp( argv[1], "-slice" ) == 0 )
	{
		_slice( argv[2], argv[3] );
		return 0;
	}



	/*!	compute the closed one form basis for multiply connected domain
	 *
	 */

	if( strcmp( argv[1], "-cohomology_one_form_domain" ) == 0 )
	{
		_cohomology_one_form_domain( argv[2], argv[3], argv[4], argv[5] );
		return 0;
	}


	/*!	compute the shortest path connecting an inner boundary to the exterior boundary
	 *
	 */
	if( strcmp( argv[1] , "-cut") == 0 )
	{
		_cut_domain( argv[2], argv[3] );
		return 0;
	}

/*---------------------------------------------------------------------------------------------------------------------------------------

	Riemann Mapping

---------------------------------------------------------------------------------------------------------------------------------------*/

	/*! Puncture a hole in the center of the mesh
	 *
	 */
  if( strcmp( argv[1], "-puncture" ) == 0 )
  {
	  _puncture( argv[2], argv[3] );
	  return 0;
  }
	/*! Fill the hole in the center
	 *
	 */
  if( strcmp( argv[1], "-fill_hole" ) == 0 )
  {
	_fill_puncture( argv[2], argv[3] );
	return 0;
  }


	help( argv[0] );
	return 0;
}
 