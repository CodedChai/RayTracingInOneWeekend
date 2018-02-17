
public class bvh_node extends hitable{
	AABB box;
	hitable left;
	hitable right;
	
	bvh_node(hitable l, int n , float t0, float t1){
		
	}
	
	@Override
	public hit_record hit(ray r, float t_min, float t_max, hit_record rec) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public AABB bounding_box(float t0, float t1, AABB box) {
		return box;
	}

}
