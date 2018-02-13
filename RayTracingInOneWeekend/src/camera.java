import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

public class camera {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		StringBuilder sb = new StringBuilder();
		int nx = 200;
		int ny = 100;
		
		sb.append("P3\n" + nx + " " + ny + "\n255\n");
		for(int j = ny-1; j >=0; j--) {
			for(int i = 0; i < nx; i++) {
				Vec3 col = new Vec3((float)i/(float)nx, (float)j/(float)ny, 0.2f);
				int ir = (int)(255.99 * col.r());
				int ig = (int)(255.99 * col.g());
				int ib = (int)(255.99 * col.b());
				sb.append(ir + " " + ig + " " + ib + "\n");
			}
		}
		
       try {  
            Writer w = new FileWriter("vecTest.ppm");  
            w.append(sb);  
            w.close();  
            System.out.println("Done");  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
	}

}
