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

}
