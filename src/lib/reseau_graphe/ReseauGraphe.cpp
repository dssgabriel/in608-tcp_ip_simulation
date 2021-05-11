#include "ReseauGraphe.hpp"
#include "machine/Routeur.hpp"
#include <cstdlib>

ReseauGraphe::ReseauGraphe() {
    m_Machines.clear();
    m_Liaisons.clear();
}

ReseauGraphe::~ReseauGraphe() {
}

// Getters & setters
void ReseauGraphe::setNom(const std::string nom) {
    m_Nom = nom;
}

const std::string& ReseauGraphe::getNom() const{
    return m_Nom;
}

Machine& ReseauGraphe::getMachine(const IPv4& ip) {
   for(size_t i = 0; i < m_Machines.size(); ++i) {
       if(m_Machines[i].getIp() == ip) {
           return m_Machines[i];
       }
   }

    std::cout << "ERREUR : Adresse IP introuvable sur le reseau.\n";
    exit(EXIT_FAILURE);
}

const std::vector<Machine>& ReseauGraphe::getMachines() const {
    return m_Machines;
}

const std::vector<Liaison>& ReseauGraphe::getLiaisons() const {
    return m_Liaisons;
}

// Overloading
std::ostream& operator<<(std::ostream& flux, const ReseauGraphe& reseau) {
    flux << "Nom de réseau : " << reseau.getNom();

    std::vector<Machine> cpyMachines = reseau.getMachines();
    std::vector<Liaison> cpyLiaisons = reseau.getLiaisons();

    for(Machine m : cpyMachines) {
        flux << m;
    }
    for(Liaison l : cpyLiaisons) {
        flux << l;
    }

    return flux;
}

// Methodes
bool ReseauGraphe::estConnexe() {
    return false;
}

void ReseauGraphe::ajouter(Machine m) {
    m_Machines.emplace_back(m);
}

void ReseauGraphe::ajouter(Liaison l) {
    m_Liaisons.emplace_back(l);
}

void ReseauGraphe::routageDynamique() {
}
