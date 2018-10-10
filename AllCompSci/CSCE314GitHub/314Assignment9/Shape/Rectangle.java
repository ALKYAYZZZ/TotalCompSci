public class Rectangle extends Shape{
    public Point point1;
    public Point point2;

    Rectangle(Point a, Point b){
        point1 = a;
        point2 = b;
    }
    //Position is center of rectangle
    public Point position(){
        double xtotal = (point1.getX() + point2.getX())/2;
        double ytotal = (point1.getY() + point2.getY())/2;
        Point newpoint = new Point(xtotal, ytotal);
        return newpoint;
    }

    public double length(){
        double length = Math.abs(point1.getX() - point2.getX());
        return length;
    }

    public double width(){
        double width = Math.abs(point1.getY() - point2.getY());
        return width;
    }
    //Area is length times width
    public double area(){
        double result = this.width() * this.length();
        System.out.println(result);
        return result;
    }
    //See if rectangles are congruent
    public boolean equals(Shape s){
        boolean result = false;
        if(s instanceof Rectangle){
            if(this.position() == s.position()){
                if(this.area() == s.area()){
                    result = true;
                }
            }
        }
        return result;
    }
    
    //Override compare functions for iterator
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
        System.out.print("Rectangle (" + point1.getX() + "," + point1.getY() + ")-(" + point2.getX() + "," + point2.getY() + ")");
        return "";
    }
}