import org.junit.Test;

/**
 * 作者：Administrator on 2016/11/20 15:25
 * 邮箱：huihui@wuliangroup.com
 */
public class Compute {
    @Test
    public void test(){
        Complex[][] constM = {
                {new Complex(1, 0),new Complex(1, 0),new Complex(1, 0),new Complex(1, 0)},
                {new Complex(1, 0),new Complex(0, 1),new Complex(-1, 0),new Complex(0, -1)},
                {new Complex(1, 0),new Complex(-1, 0),new Complex(1, 0),new Complex(-1, 0)},
                {new Complex(1, 0),new Complex(0, -1),new Complex(-1, 0),new Complex(0, 1)}};
        int count = 0;
        for(int i = 0; i < 4; ++i){
            for (int j = 0; j < 4; ++j){
                for(int k = 0; k < 4; ++k) {
                    for(int l = 0; l < 4; ++l){
                        Complex out = constM[i][0].mul(constM[j][1]).mul(constM[k][2]).mul(constM[l][3])
                                .add((constM[i][1]).mul(constM[j][2]).mul(constM[k][3]).mul(constM[l][0]))
                                .add((constM[i][2]).mul(constM[j][3]).mul(constM[k][0]).mul(constM[l][1]))
                                .add((constM[i][3]).mul(constM[j][0]).mul(constM[k][1]).mul(constM[l][2]));
                        if(out.getReal() != 0 || out.getImage() != 0){
                            count++;
                        }
                        System.out.print("out[" + i + j + k + l + "]:" + (int)out.getReal() + "+" + (int)out.getImage() + "i\n");
                    }
                }
            }
        }
        System.out.print("not zero is " + count);
    }
}
