public class Point{
        public double xmain;
        public double ymain;

        Point(){
            xmain = 0.0;
            ymain = 0.0;
        }

        Point(double x, double y){
            xmain = x;
            ymain = y;
        }

        public double getX(){
            return xmain;
        }

        public double getY(){
            return ymain;
        }
}