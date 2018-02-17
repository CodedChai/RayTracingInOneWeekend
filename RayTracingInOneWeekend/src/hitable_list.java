import java.util.*;

public class hitable_list extends hitable {
	
	ArrayList<hitable> list;
	int list_size;
	
	hitable_list(hitable[] l, int n){
		list = new ArrayList<hitable>(n);
		for(int i = 0; i < n; i++) {
			list.add(l[i]);
		}
		list_size = n;
	}
	
	@Override
	public hit_record hit(ray r, float t_min, float t_max, hit_record rec) {
		hit_record curr_rec = new hit_record();
		boolean hit_anything = false;
		float closest_so_far = t_max;
		for(int i = 0; i < list_size; i++) {
			curr_rec = list.get(i).hit(r, t_min, closest_so_far, curr_rec);
			if(curr_rec.valid) {
				hit_anything = true;
				closest_so_far = curr_rec.t;
				curr_rec.mat = list.get(i).mat;
				rec = new hit_record(curr_rec);

			}
		}
		
		return rec;
	}
	
	public AABB surrounding_box(AABB box0, AABB box1){
		Vec3 small = new Vec3(ffmin(box0._min.x(), box1._min.x()),
				ffmin(box0._min.y(), box1._min.y()),
				ffmin(box0._min.z(), box1._min.z()));

		Vec3 big = new Vec3(ffmax(box0._max.x(), box1._max.x()),
				ffmax(box0._max.y(), box1._max.y()),
				ffmax(box0._max.z(), box1._max.z()));

		return new AABB(small, big);
	}

	private float ffmin(float a, float b){
		return a < b ? a : b;
	}

	private float ffmax(float a, float b){
		return a > b ? a : b;
	}
	
	
	// Very not sure about this method
	@Override
	public AABB bounding_box(float t0, float t1, AABB box) {
		if(list_size < 1) {
			return null;
		}
		
		AABB temp_box = new AABB();
		
		AABB first_true = list.get(0).bounding_box(t0, t1, temp_box);
		if(first_true == null) {
			return null;
		} else {
			box = temp_box;
		}
		
		for(int i = 1; i < list.size(); i++) {
			if(list.get(0).bounding_box(t0,  t1, temp_box) != null) {
				box = surrounding_box(box, temp_box);
			} else {
				return null;
			}
		}
		return box;
	}

}








