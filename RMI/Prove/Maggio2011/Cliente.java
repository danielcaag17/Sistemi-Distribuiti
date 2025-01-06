package RMI.Prove.Maggio2011;

import java.rmi.Naming;

public class Cliente {
    public static void main(String[] args) {
        try {
            StorageService servicioC = (StorageService) Naming.lookup("//localhost/StorageService");

            // Obtener los nombres de los servicios
            String readServiceName = servicioC.getNameService("ServerB");
            String writeServiceName = servicioC.getNameService("ServerA");

             // Obtener las referencias a los servicios
             ReadService readService = (ReadService) Naming.lookup("//localhost/" + readServiceName);
             WriteService writeService = (WriteService) Naming.lookup("//localhost/" + writeServiceName);
 
             // Leer el contenido del archivo en el servidor B
             String content = readService.readFile("vecchio.txt");
 
             // Escribir el contenido en el archivo en el servidor A
             writeService.writeFile("nuovo.txt", content);

        } catch (Exception e) {
            System.out.println(e);
        }
        
    }
}
