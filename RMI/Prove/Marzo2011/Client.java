package RMI.Prove.Marzo2011;

import java.rmi.Naming;

public class Client {
    public static void main(String[] args) {
        try {
            String clientId = "Cliente-" + System.currentTimeMillis();
            ResourceManager resourceManager = (ResourceManager) Naming.lookup("//localhost/ResourceManagerImpl");
            FileService fileService = (FileService) Naming.lookup("//localhost/FileServiceImpl");

            System.out.println("Solicitando acceso...");
            while (!resourceManager.requestAccess(clientId)) {
                Thread.sleep(1000); // Espera antes de volver a intentar
            }

            System.out.println("Acceso concedido, escribiendo 5 cadenas...");
            for (int i = 0; i < 5; i++) {
                fileService.writeToFile(clientId + ": Línea " + (i + 1));
                Thread.sleep(500); // Simula tiempo entre escrituras
            }

            System.out.println("Liberando recurso...");
            resourceManager.releaseAccess(clientId);

        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
