package snippet;

import java.util.HashSet;
import java.util.Set;

public class TetramerCal {
	final static int dimension = 16;
	final static int hor_dimension = 4;

	public static void main(String[] args) {
		int[][][][] matr = new int[dimension][dimension][dimension][dimension];
		for (int i = 0; i < dimension; ++i) {
			for (int j = 0; j < dimension; ++j) {
				for (int k = 0; k < dimension; ++k) {
					for (int l = 0; l < dimension; ++l) {
						if(matchGroup1(i,j,k,l) || matchGroup2(i,j,k,l)){
							matr[i][j][k][l] = 1;
							System.out.print("matrix[" + i + "]" + "[" + j + "]"+ "[" + k + "]"+ "[" + l + "]=" + matr[i][j][k][l] );
						}
					}
					System.out.print("\n");
				}
				System.out.print("\n");
			}
			System.out.print("\n");
		}
		
		int[] indexS0 = new int[dimension];
		int[] indexS1 = new int[dimension];
		int[] indexS2 = new int[dimension];
		int[] indexS3 = new int[dimension];

		for (int i = 0; i < dimension; ++i) {
			indexS0[i] = i;
		}
		int[][] nextIndexS = getNext(indexS0);
		int sigma = 0;
		for (int i = 0; i < dimension; ++i) {
			for (int j = 0; j < dimension; ++j) {
				indexS1[i] = j;
			}
		}
		Set<Integer> set = new HashSet<Integer>();  
		
		for(int i = 0; i < dimension; ++i){
			set.add(indexS0[i]);
		}
	}
	

	static int[][] getNext(int[] pre){
		int[][] tem = new int[dimension][2];
		for (int i = 0; i < dimension; ++i) {
			tem[i][0] = (i+1)%dimension;
			tem[i][1] = (i+1*hor_dimension)%dimension;
		}
		return tem;
	}
	
	static boolean matchGroup1(int i, int j, int k, int l){
		if(i+3*hor_dimension == j+2*hor_dimension && k+1*hor_dimension == l && i+3*hor_dimension == l){
			return true;
		}
		if(j+3*hor_dimension == k+2*hor_dimension && l+1*hor_dimension == i && j+3*hor_dimension == i){
			return true;
		}
		if(k+3*hor_dimension == l+2*hor_dimension && i+1*hor_dimension == j && k+3*hor_dimension == j){
			return true;
		}
		if(l+3*hor_dimension == i+2*hor_dimension && j+1*hor_dimension == k && l+3*hor_dimension == k){
			return true;
		}
		return false;
	}
	
	static boolean matchGroup2(int i, int j, int k, int l){
		if(i+3 == j+2 && k+1 == l && i+3 == l){
			return true;
		}
		if(j+3 == k+2 && l+1 == i && j+3 == i){
			return true;
		}
		if(k+3 == l+2 && i+1 == j && k+3 == j){
			return true;
		}
		if(l+3 == i+2 && j+1 == k && l+3 == k){
			return true;
		}
		return false;
	}
}
