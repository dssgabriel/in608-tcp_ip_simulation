/**
 * @file        Physique.hpp
 * @brief       Déclaration de la classe Physique.
 * 
 * @author      Quentin GRUCHET
 * @author      Fadi MECHRI
 * @date        2021
 */


#pragma once

#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <stack>

#include "../../../include/MAC.hpp"
#include "../../../include/Commun.hpp"

class Physique {
    private :
        // Attributs
        MAC m_SrcMac;
        MAC m_DestMac;

    public :
       
        // Constructeur
        Physique();
       
        // Destructeur
        ~Physique();
       
        // Getters & setters
        void setMacSrc(MAC src);
        const MAC getMacSrc() const;
        
        void setMacDest(MAC dest);
        const MAC getMacDest() const;
        
        // Methodes
        MAC convertir(
            const std::bitset<16>& adrPartBA, 
            const std::bitset<16>& adrPartDC,
            const std::bitset<16>& adrPartFE
        );

        std::bitset<48> convertir(const MAC& adresse);
        
        std::stack<std::bitset<16>> encapsuler(
            std::stack<std::bitset<16>>& paquet
        );

        std::stack<std::bitset<16>> desencapsuler(
            std::stack<std::bitset<16>>& trame
        );

        // Overloading
        friend std::ostream& operator<<(
            std::ostream& flux, 
            const Physique& couchePhy
        );
};