import java.io.*;
public class camera {

	public static void main(String[] args) {
		StringBuilder sb = new StringBuilder();
		int nx = 1920;
		int ny = 1080;

		Vec3 lower_left_corner = new Vec3(-2.0f, -1.0f, -1.0f);
		Vec3 horizontal = new Vec3(4.0f, 0.0f, 0.0f);
		Vec3 vertical = new Vec3(0.0f, 2.0f, 0.0f);
		Vec3 origin = new Vec3(0.0f, 0.0f, 0.0f);
		
		// Create all hitable spheres
		hitable[] list = new hitable[2];
		list[0] = new sphere(new Vec3(0.0f, 0.0f, -1.0f),0.5f);
		list[1] = new sphere(new Vec3(0.0f, -100.5f, -1.0f), 100);
		hitable_list world = new hitable_list(list, 2);
		
		sb.append("P3\n" + nx + " " + ny + "\n255\n");
		for(int j = ny-1; j >=0; j--) {
			for(int i = 0; i < nx; i++) {
				float u = (float)(i)/(float)(nx);
				float v = (float)(j)/(float)(ny);
				
				// lower left corner + u * horizontal _ v * vertical
				ray r = new ray(origin, horizontal.mul(u).add(vertical.mul(v)).add(lower_left_corner));
				
				Vec3 col = color(r, world);
				int ir = (int)(255.99 * col.r());
				int ig = (int)(255.99 * col.g());
				int ib = (int)(255.99 * col.b());
				sb.append(ir + " " + ig + " " + ib + "\n");
			}
		}
		
       try {  
            Writer w = new FileWriter("WorldNormalSphere.ppm");  
            w.append(sb);  
            w.close();  
            System.out.println("Done");  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
	}
	
	
	private static Vec3 color(ray r, hitable_list world) {
		hit_record rec = new hit_record();
		if(world.hit(r,  0.0f, Float.MAX_VALUE, rec)) {
			// Change the -1 to 1 coordinates to 0 to 1
			// 0.5f * new Vec3(N.x() + 1f, N.y() + 1f, N.z() + 1f)
			return rec.normal.add(1f).mul(.5f);
		} else {
			Vec3 unit_direction = r.direction().normalize();
			float t = (float) (0.5f * (unit_direction.y() + 1.0));
			Vec3 white = new Vec3(1.0f, 1.0f, 1.0f);
			Vec3 blue = new Vec3(0.5f, 0.7f, 1.0f);
			return white.mul(1.0f - t).add(blue.mul(t));
		}
	}
}
