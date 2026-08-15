#pragma once

/*
    Esta clase representa el menú de pausa que aparece cuando
    se pulsa la tecla de salida durante un combate
*/
class MenuPausa {
    private:
        // El constructor es privado porque la clase es Singleton
        MenuPausa();

        ~MenuPausa();

        // La única instancia
        static MenuPausa * menuPausa;

    public:

        // Comienza el funcionamiento del menú de pausa
        void comenzar();

        // NUNCA SE COPIA UN SINGLETON
        MenuPausa(MenuPausa &otro) = delete;

        // NUNCA SE ASIGNA UN SINGLETON
        void operator=(const MenuPausa &) = delete;

        // Devuelve la única instancia
        static MenuPausa * unicaInstancia();
};