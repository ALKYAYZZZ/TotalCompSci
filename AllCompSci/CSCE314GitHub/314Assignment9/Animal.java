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

public class Dog extends Animal{
    public String cry(){
        return "Bark!";
    }
}

public class Cat extends Animal{
    public String cry(){
        return "Meow!";
    }
}

public class AnimalShelter extends Animal{
    public String cry(){
        return "";
    }


ArrayList<Animal> petList = new ArrayList<Animal>();

public void addAnimal(String s){
    if(s.charAt(0) == 'd'){
        Dog tempDog = new Dog();
        tempDog.cry();
        tempDog.setName("Dog");
        petList.add(tempDog);
    }
    if(s.charAt(0) == 'c'){
        Cat tempCat = new Cat();
        tempCat.cry();
        tempCat.setName("Cat");
        petList.add(tempCat);
    }
}

public void adopt(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    petList.remove(0);
}

public void adoptCat(){
    for(int i = 0; i < petList.size(); i++){
        if(petList.get(i).getName() == "Cat"){
            petList.remove(i);
            break;
        }
    }
}

public void adoptDog(){
    for(int i = 0; i < petList.size(); i++){
        if(petList.get(i).getName() == "Dog"){
            petList.remove(i);
            break;
        }
    }
}

public void remainingAnimals(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    else{
        for(int i = 0; i < petList.size(); i++){
            System.out.print(petList.get(i).getName() + "|" + petList.get(i).cry() + "|"  + "Order: " + i);
        }
    }
}

public void remainingDogs(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    else{
        if(petList.get(i).getName() == "Dog"){
            for(int i = 0; i < petList.size(); i++){
                System.out.print(petList.get(i).getName() + "|" + petList.get(i).cry() + "|"  + "Order: " + i);
            }
        }
    }
}

public void remainingCats(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    else{
        if(petList.get(i).getName() == "Cat"){
            for(int i = 0; i < petList.size(); i++){
                System.out.print(petList.get(i).getName() + "|" + petList.get(i).cry() + "|"  + "Order: " + i);
            }
        }
    }

}
}