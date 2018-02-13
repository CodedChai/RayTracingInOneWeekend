import java.io.*;
public class camera {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		StringBuilder sb = new StringBuilder();
		int nx = 1920;
		int ny = 1080;

		Vec3 lower_left_corner = new Vec3(-2.0f, -1.0f, -1.0f);
		Vec3 horizontal = new Vec3(4.0f, 0.0f, 0.0f);
		Vec3 vertical = new Vec3(0.0f, 2.0f, 0.0f);
		Vec3 origin = new Vec3(0.0f, 0.0f, 0.0f);
		
		sb.append("P3\n" + nx + " " + ny + "\n255\n");
		for(int j = ny-1; j >=0; j--) {
			for(int i = 0; i < nx; i++) {
				float u = (float)(i)/(float)(nx);
				float v = (float)(j)/(float)(ny);
				
				// lower left corner + u * horizontal _ v * vertical
				ray r = new ray(origin, horizontal.mul(u).add(vertical.mul(v)).add(lower_left_corner));
				
				Vec3 col = color(r);
				int ir = (int)(255.99 * col.r());
				int ig = (int)(255.99 * col.g());
				int ib = (int)(255.99 * col.b());
				sb.append(ir + " " + ig + " " + ib + "\n");
			}
		}
		
       try {  
            Writer w = new FileWriter("NormalSphere.ppm");  
            w.append(sb);  
            w.close();  
            System.out.println("Done");  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
	}
	
	
	private static float hit_sphere(Vec3 center, float radius, ray r) {
		// r.origin - center
		Vec3 oc = r.origin().sub(center);
		
		float a = r.direction().dot( r.direction());
		float b = 2.0f * oc.dot(r.direction());
		float c = oc.dot(oc) - radius * radius;
		float discriminant = b * b - 4 * a * c;
		
		if (discriminant < 0) {
			return -1.0f;
		} else {
			// return normal: direction of hitpoint minus center
			return (float) ((-b - Math.sqrt(discriminant))/(2.0f * a));
		}
	}
	
	private static Vec3 color(ray r) {
		float t = hit_sphere(new Vec3(0f, 0f, -1f), 0.5f, r);
		if(t > 0.0f){
			Vec3 N = r.point_at_parameter(t).sub(new Vec3(0f, 0f, -1f)).normalize();
			// Change the -1 to 1 coordinates to 0 to 1
			// 0.5f * new Vec3(N.x() + 1f, N.y() + 1f, N.z() + 1f)
			return N.add(1f).mul(.5f);
		}
		
		Vec3 unit_direction = r.direction().normalize();
		t = (float) (0.5f * (unit_direction.y() + 1.0));
		Vec3 white = new Vec3(1.0f, 1.0f, 1.0f);
		Vec3 blue = new Vec3(0.5f, 0.7f, 1.0f);
		return white.mul(1.0f - t).add(blue.mul(t));
	}

}
