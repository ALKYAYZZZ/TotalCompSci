public class AreaCalculator{
    public static double calculate(Shape[] shapes){
        double total = 0;
        for(int i = 0; i < shapes.length; i++){
            total = total + shapes[i].area();
        }

        System.out.println(total);
        return total;
    }

    public double area(){
        return 1;
    }
}