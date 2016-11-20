/**
 * 作者：Administrator on 2016/11/20 15:24
 * 邮箱：huihui@wuliangroup.com
 */

public class Complex { // 复数类
    double real;  // 实部
    double image; // 虚部

    Complex(double real,double image){ // 带参数的构造方法
        this.real = real;
        this.image = image;
    }

    public Complex() {
        // TODO Auto-generated constructor stub
    }

    public double getReal() {
        return real;
    }

    public void setReal(double real) {
        this.real = real;
    }

    public double getImage() {
        return image;
    }

    public void setImage(double image) {
        this.image = image;
    }

    Complex add(Complex a){ // 复数相加
        double real2 = a.getReal();
        double image2 = a.getImage();
        double newReal = real + real2;
        double newImage = image + image2;
        Complex result = new Complex(newReal,newImage);
        return result;
    }

    Complex sub(Complex a){ // 复数相减
        double real2 = a.getReal();
        double image2 = a.getImage();
        double newReal = real - real2;
        double newImage = image - image2;
        Complex result = new Complex(newReal,newImage);
        return result;
    }

    Complex mul(Complex a){ // 复数相乘
        double real2 = a.getReal();
        double image2 = a.getImage();
        double newReal = real*real2 - image*image2;
        double newImage = image*real2 + real*image2;
        Complex result = new Complex(newReal,newImage);
        return result;
    }

    Complex div(Complex a){ // 复数相除
        double real2 = a.getReal();
        double image2 = a.getImage();
        double newReal = (real*real2 + image*image2)/(real2*real2 + image2*image2);
        double newImage = (image*real2 - real*image2)/(real2*real2 + image2*image2);
        Complex result = new Complex(newReal,newImage);
        return result;
    }

    public void print(){ // 用于格式化输出
        if(image > 0){
            System.out.println(real + " + " + image + "i");
        }else if(image < 0){
            System.out.println(real + "" + image + "i");
        }else{
            System.out.println(real);
        }
    }

    // 封装了具体运算，主要为对输入进行转换，对输出封装
    public static void talk(String data1_1,String data1_2,String data2_1,String data2_2,String operation){

        // 以下语句为格式转换
        double dat1_1 = Double.parseDouble(data1_1); // 把string型实部转为double型
        double dat1_2 = Double.parseDouble(data1_2.substring(0,data1_2.length()-1));// 把去除"i"的string型虚部转为double型
        double dat2_1 = Double.parseDouble(data2_1);// 把string型实部转为double型
        double dat2_2 = Double.parseDouble(data2_2.substring(0,data2_2.length()-1));// 把去除"i"的string型虚部转为double型

        Complex num1 = new Complex(dat1_1,dat1_2);
        Complex num2 = new Complex(dat2_1,dat2_2);

        Complex result;
        int a,b;

        if(operation.equals("+")){ // 两数相加
            result = num1.add(num2);
            result.print();
        }

        if(operation.equals("-")){ // 两数相减
            result = num1.sub(num2);
            result.print();
        }

        if(operation.equals("*")){ // 两数相乘
            result = num1.mul(num2);
            result.print();
        }

        if(operation.equals("/")){ // 两数相除

            if(num2.real * num2.real + num2.image*num2.image != 0){
                result = num1.div(num2);
                result.print();
            }else{
                System.out.println("运算出错,除数运算中除数不能为0,请用户重新输入!");
            }
        }
    }
}