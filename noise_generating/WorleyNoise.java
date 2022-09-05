package noise;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;


/**
 * 
 * @author Jannis Welkener
 * @info Seamless 3D-Worley-Noise for Computer Graphics Course @ HSOS, finished 17.07.2022
 *
 */


public class WorleyNoise {
	
	static final int numberOfPoints=500;
	static final int textureSizePxP=64;
	static ArrayList<WorleyPoint> pList=new ArrayList<>();
	static ArrayList<WorleyPoint> initialSetOfPoints=new ArrayList<>();
	
	public static void main(String[] args) {

		for(int i=0;i<numberOfPoints;i++) {
			double x = Math.random() * textureSizePxP;
			double y = Math.random() * textureSizePxP;
			double z = Math.random() * textureSizePxP;
			WorleyPoint p = new WorleyPoint(x, y, z);
			initialSetOfPoints.add(p);
		}
		
		
		for(int i=0;i<numberOfPoints;i++) {
			pList.add(initialSetOfPoints.get(i));	
		}
		
		for(int i=0;i<128;i++) {
			WorleyPoint activePoint = initialSetOfPoints.get(i);

			pList.add(new WorleyPoint(activePoint.x + textureSizePxP,activePoint.y,activePoint.z));	
			pList.add(new WorleyPoint(activePoint.x + textureSizePxP*2,activePoint.y,activePoint.z));	
			
			pList.add(new WorleyPoint(activePoint.x + textureSizePxP,activePoint.y + textureSizePxP,activePoint.z));	
			pList.add(new WorleyPoint(activePoint.x + textureSizePxP*2,activePoint.y + textureSizePxP,activePoint.z));

			pList.add(new WorleyPoint(activePoint.x + textureSizePxP,activePoint.y + textureSizePxP*2,activePoint.z));	
			pList.add(new WorleyPoint(activePoint.x + textureSizePxP*2,activePoint.y + textureSizePxP*2,activePoint.z));	
			
			pList.add(new WorleyPoint(activePoint.x,activePoint.y + textureSizePxP,activePoint.z));	
			pList.add(new WorleyPoint(activePoint.x,activePoint.y + textureSizePxP*2,activePoint.z));
		}
		
		
		

		try {
			deleteDirectoryRecursionJava6(new File("G:/Eclipse/Workspace/Graphics/noise"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		new File("G:/Eclipse/Workspace/Graphics/noise").mkdirs();

		

		for(int k=0;k<textureSizePxP;k++) {

			double distance[][]=new double[textureSizePxP*3][textureSizePxP*3];
			double highestVal=Double.MIN_VALUE;
			double lowestVal=Double.MAX_VALUE;

			for(int i=0;i<textureSizePxP*3;i++) {
				for(int j=0;j<textureSizePxP*3;j++) {


					double nearestD=Double.MAX_VALUE;
					for(int i_=0;i_<pList.size();i_++) {

						WorleyPoint curr = pList.get(i_);
						double dx=curr.x-i;
						double dy=curr.y-j;
						double dz=curr.z-k;

						double dist = Math.sqrt( (dx*dx)+(dy*dy)+(dz*dz));

						if(dist<nearestD)nearestD=dist;
					}
					if(nearestD>highestVal)highestVal=nearestD;
					if(nearestD<lowestVal)lowestVal=nearestD;
					distance[i][j]=nearestD;
				}
			}

			//System.out.println(highestVal);
			//System.out.println(lowestVal);

			BufferedImage image=new BufferedImage(textureSizePxP, textureSizePxP, BufferedImage.TYPE_INT_RGB);	
			for(int i=0;i<textureSizePxP;i++) {
				for(int j=0;j<textureSizePxP;j++) {	
					float wbValNormalized = (float) ((distance[i+textureSizePxP][j+textureSizePxP] - lowestVal) / (highestVal-lowestVal));
					float wbVal = 1.0f - wbValNormalized;
					image.setRGB(i, j, new Color(wbVal,wbVal,wbVal).getRGB());
				}
			}

			try {
				ImageIO.write(image, "BMP", new File("G:/Eclipse/Workspace/Graphics/noise/worley"+k+".bmp"));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		System.out.println("finished.");

	}
	
	static void deleteDirectoryRecursionJava6(File file) throws IOException {
		if (file.isDirectory()) {
			File[] entries = file.listFiles();
			if (entries != null) {
				for (File entry : entries) {
					deleteDirectoryRecursionJava6(entry);
				}
			}
		}
		if (!file.delete()) {
			throw new IOException("Failed to delete " + file);
		}
	}
	
}
