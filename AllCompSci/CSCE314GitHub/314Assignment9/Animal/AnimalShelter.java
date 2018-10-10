import java.util.*;

public class AnimalShelter extends Animal{
    public String cry(){
        return "";
    }


ArrayList<Animal> petList = new ArrayList<Animal>();
//Add dog or cat depedning on user input
public void addAnimal(String s){
    if(s.charAt(0) == 'd'){
		String result = s.substring(2);
        Dog tempDog = new Dog();
        tempDog.cry();
        tempDog.setName(result);
        petList.add(tempDog);
    }
    if(s.charAt(0) == 'c'){
		String result = s.substring(2);
        Cat tempCat = new Cat();
        tempCat.cry();
        tempCat.setName(result);
        petList.add(tempCat);
    }
}
//Remove animal
public void adopt(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    petList.remove(0);
}
//Remove Cat
public void adoptCat(){
    for(int i = 0; i < petList.size(); i++){
        if(petList.get(i) instanceof Cat){
            petList.remove(i);
            break;
        }
    }
}
//Remove Dog
public void adoptDog(){
    for(int i = 0; i < petList.size(); i++){
        if(petList.get(i) instanceof Dog){
            petList.remove(i);
            break;
        }
    }
}
//Count all remaining animals using looping
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
//Count all animals that are dogs using looping
public void remainingDogs(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    else{
        
        for(int i = 0; i < petList.size(); i++){
            if(petList.get(i) instanceof Dog){
                System.out.print(petList.get(i).getName() + "|" + petList.get(i).cry() + "|"  + "Order: " + i);
            }
        }
    }
}
//Count all animals that are cats
public void remainingCats(){
    if(petList.size() == 0){
        System.out.println("There are no animals in this list!");
    }
    else{
        for(int i = 0; i < petList.size(); i++){
            if(petList.get(i) instanceof Cat){
                System.out.print(petList.get(i).getName() + "|" + petList.get(i).cry() + "|"  + "Order: " + i);
            }
        }
    }

}
}