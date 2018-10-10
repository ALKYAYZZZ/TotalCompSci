import java.util.*;

public abstract class Animal{
    public int order;
    public String name;

    public String getName() {return name;}
    public int getOrder() {return order;}

    public abstract String cry();

    public void setName(String n){
        name = n;
    }

}
