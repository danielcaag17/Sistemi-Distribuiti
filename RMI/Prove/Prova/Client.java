package RMI.Prove.Prova;

import java.rmi.Naming;

public class Client {
    public static void main(String[] args) {
        try {
            String name = "name";
            String surname = "surname";
            String matriculationNumber = "UNI120";
            int courseCode1 = 123;
            int courseCode2 = 321;

            UniService uniService = (UniService) Naming.lookup("//localhost/UniServiceImpl");
            String id1 = uniService.register(name, surname, matriculationNumber, courseCode1);
            String id2 = uniService.register(name, surname, matriculationNumber, courseCode2);

            int c1 = uniService.howManyRegistered(courseCode1);
            int c2 = uniService.howManyRegistered(courseCode2);

            if (c1 > c2) uniService.deleteRegistrarion(id1);
            else uniService.deleteRegistrarion(id2);
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}