
public class hit_record {

	public hit_record(hit_record temp_rec) {
		t = temp_rec.t;
		p = temp_rec.p;
		normal = temp_rec.normal;
		valid = temp_rec.valid;
	}
	
	public hit_record() {
		t = 1f;
		p = Vec3.zero();
		normal = Vec3.zero();
		valid = false;
	}
	
	public float t;
	public Vec3 p;
	public Vec3 normal;
	public boolean valid;
	
}
