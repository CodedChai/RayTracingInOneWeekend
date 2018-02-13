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
	public boolean hit(ray r, float t_min, float t_max, hit_record rec) {
		hit_record temp_rec = new hit_record();
		boolean hit_anything = false;
		float closest_so_far = t_max;
		for(int i = 0; i < list_size; i++) {
			if(list.get(i).hit(r, t_min, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		
		return hit_anything;
	}

}
