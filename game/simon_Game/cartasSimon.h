enum naipe
{
    COPAS,
    ESPADAS,
    DIAMANTES,
    PAUS
};

struct carta{
    enum naipe naipe;
    int valor; 
};