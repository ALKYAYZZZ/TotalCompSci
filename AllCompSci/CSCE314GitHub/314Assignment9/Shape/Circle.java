public class Circle extends Shape{
    private Point center;
    private double radius;

    Circle(Point c, double r){
        center = c;
        radius = r;
    }
    //Position is center of circle
    public Point position(){
        return center;
    }
    //Normal area function is pi r^2
    public double area(){
        double result = 3.14 * radius * radius;
        System.out.println(result);
        return result;
    }
    //
    public boolean equals(Shape s){
        boolean result = false;
        if(s instanceof Circle){
            if(this.position() == s.position()){
                if(this.area() == s.area()){
                    result = true;
                }
            }
        }
        return result;
    }
    //Override compare operator for iterator
    @Override
    public int compareTo(Shape s){
        if(this.area() > s.area()){
            return 1;
        }
        else if(this.area() < s.area()){
            return -1;
        }
        else{
            return 0;
        }
    }

    public String toString(){
        System.out.print("Circle (" + center.getX() + "," + center.getY() + ") Radius - " + radius);
        return "";
    }
}