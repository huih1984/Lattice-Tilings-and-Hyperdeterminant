

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Main {
    final static int dimension = 16;
    final static int hor_dimension = 4;

    public static void main(String[] args) {
        int[][][][] matr = new int[dimension][dimension][dimension][dimension];
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                for (int k = 0; k < dimension; ++k) {
                    for (int l = 0; l < dimension; ++l) {
                        if (matchGroup1(i, j, k, l) || matchGroup2(i, j, k, l)) {
                            matr[i][j][k][l] = 1;
                            System.out.print("matrix[" + i + "]" + "[" + j + "]" + "[" + k + "]" + "[" + l + "]=" + matr[i][j][k][l]);
                        }
                    }
                    System.out.print("\n");
                }
            }
        }

        int[] indexS0 = new int[dimension];
        int[] indexS1 = new int[dimension];

        for (int i = 0; i < dimension; ++i) {
            indexS0[i] = i;
        }
        int[][] nextIndexS = getNext(indexS0);
        indexpa(0, indexS0, indexS1, nextIndexS);


        System.out.print(looplen);


    }

     static List<int[]> list = new ArrayList<int[]>();

    static int looplen = 0;

    static boolean regal(int[] indexS) {
        Set<Integer> set = new HashSet<Integer>();

        for (int j = 0; j < dimension; ++j) {
            if(indexS[j] == -1){
                return false;
            }
            set.add(indexS[j]);
        }
        return set.size() == dimension;
    }

    static int getSign(int[] list){
        int sign = 1;
       for(int i = 0; i < list.length; ++i){
           for(int j = i + 1; j < list.length; ++j) {
               if(list[j] < list[i]){
                    sign *= -1;
               }
           }
       }
        return sign;
    }

    static void indexpa(int ind, int[] indexS0, int[] indexS1, int[][] nextIndexS) {
        for (int i = 0; i < 2; ++i) {
            indexS1[ind] = nextIndexS[ind][i];
            if (ind == dimension - 1) {

                if (regal(indexS1)) {

                    int[] indexS2 = new int[dimension];
                    int[] indexS3 = new int[dimension];
                    for (int s = 0; s < dimension; ++s) {
                        System.out.print(" " + " " + indexS1[s]);
                        int delta =Math.abs((indexS1[s] - indexS0[s])%4) == 0?4:1;
                        indexS2[s] = (((int)(indexS0[s]/(4*delta)))*(4*delta) + (indexS0[s]%(4*delta) + (2*delta))%(4*delta))%16;
                        indexS3[s] = (((int)(indexS0[s]/(4*delta)))*(4*delta) + (indexS0[s]%(4*delta) + (3*delta))%(4*delta))%16;
                    }
//                    looplen ++;
                    looplen += getSign(indexS1)*getSign(indexS2)*getSign(indexS3);
                    System.out.print("\n");
                } else {
                }
            } else {
                indexpa(ind + 1, indexS0, indexS1, nextIndexS);
            }
        }
    }

    static boolean indexpa1(int ind, int[] indexS0, int[] indexS1, int[] indexS2, int[][] nextIndexS) {

        for (int i = 0; i < 2; ++i) {
            indexS2[ind] = nextIndexS[ind][i];
            if (ind == dimension - 1) {
                Set<Integer> set = new HashSet<Integer>();

                for (int j = 0; j < dimension; ++j) {
                    set.add(indexS2[j]);
                }

                if (set.size() == dimension) {
                    boolean regel = true;
                    for (int s = 0; s < dimension; ++s) {
                        if (!(Math.abs(indexS2[s] - indexS1[s]) + Math.abs(indexS1[s] - indexS0[s]) == 4 || Math.abs(indexS2[s] - indexS1[s]) + Math.abs(indexS1[s] - indexS0[s]) == 2
                                || Math.abs(indexS2[s] - indexS1[s]) + Math.abs(indexS1[s] - indexS0[s]) == 2 * hor_dimension
                                || Math.abs(indexS2[s] - indexS1[s]) + Math.abs(indexS1[s] - indexS0[s]) == 4 * hor_dimension)) {

                            regel = false;
                            break;
                        }
                    }
                    if (regel) {
                        for (int s = 0; s < dimension; ++s) {
                            System.out.print("" + indexS0[s] + " " + indexS1[s] + " " + indexS2[s] + "\n");
                        }
                        System.out.print("\n");
                        looplen++;
                    }
                    return true;
                } else {
                    return false;
                }
            } else {
                indexpa1(ind + 1, indexS0, indexS1, indexS2, nextIndexS);
            }
        }
        return false;
    }


    static int[][] getNext(int[] pre) {
        int[][] tem = new int[dimension][2];
        for (int i = 0; i < dimension; ++i) {
            tem[i][0] = (int) ((pre[i] / hor_dimension - 1) * hor_dimension + ((pre[i] - (pre[i] / hor_dimension - 1) * hor_dimension) % 4 + 1) % 4) + 4;
            tem[i][1] = (pre[i] + 1 * hor_dimension)%(4*hor_dimension) ;
//            if(tem[i][1] > 19){
//                tem[i][1] -= (4*hor_dimension)    ;
//            }
//            if(i > 11 && i < 16){
//                tem[i][2] = i - 3*hor_dimension ;
//            } else{
//                tem[i][2] = -1;
//            }

        }
        return tem;
    }

    static boolean matchGroup1(int i, int j, int k, int l) {
        if (i + 3 * hor_dimension == j + 2 * hor_dimension && k + 1 * hor_dimension == l && i + 3 * hor_dimension == l) {
            return true;
        }
        if (j + 3 * hor_dimension == k + 2 * hor_dimension && l + 1 * hor_dimension == i && j + 3 * hor_dimension == i) {
            return true;
        }
        if (k + 3 * hor_dimension == l + 2 * hor_dimension && i + 1 * hor_dimension == j && k + 3 * hor_dimension == j) {
            return true;
        }
        if (l + 3 * hor_dimension == i + 2 * hor_dimension && j + 1 * hor_dimension == k && l + 3 * hor_dimension == k) {
            return true;
        }
        return false;
    }

    static boolean matchGroup2(int i, int j, int k, int l) {
        if (i + 3 == j + 2 && k + 1 == l && i + 3 == l) {
            return true;
        }
        if (j + 3 == k + 2 && l + 1 == i && j + 3 == i) {
            return true;
        }
        if (k + 3 == l + 2 && i + 1 == j && k + 3 == j) {
            return true;
        }
        if (l + 3 == i + 2 && j + 1 == k && l + 3 == k) {
            return true;
        }
        return false;
    }
}
