#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "machine/Machine.hpp"
#include "machine/Routeur.hpp"
#include "machine/Commutateur.hpp"
#include "../../include/Liaison.hpp"

class ReseauGraphe {
    private:
        // Attributs
        std::string m_Nom;
        static std::vector<Machine> m_Machines;
        static std::vector<Liaison> m_Liaisons;

        static uint8_t getRouteur(const uint16_t idMachine);
        static std::vector<Liaison> getCheminsVoisins(const uint8_t& routeurCourant);
        static void getPlusCourtChemin(
            const uint8_t& depart,
            const uint8_t& arrivee,
            std::vector<int16_t> peres,
            std::vector<Liaison> plusCourtChemin
        );

    public:
        // Constructeur
        ReseauGraphe();

        // Destructeur
        ~ReseauGraphe();

        // Getters & setters
        void setNom(const std::string nom);
        const std::string& getNom() const;

        static Routeur getRouteur(const uint8_t idRouteur);
        Machine& getMachine(const IPv4& ip);
        const std::vector<Machine>& getMachines() const;
        const std::vector<Liaison>& getLiaisons() const;

        // Overloading
        friend std::ostream& operator<<(
            std::ostream& flux,
            const ReseauGraphe& reseau
        );

        // Methodes
        bool estConnexe();

        void ajouter(Machine m);
        void ajouter(Liaison l);

        static std::vector<Liaison> routageDynamique(const uint8_t depart, const uint8_t arrivee);
};
