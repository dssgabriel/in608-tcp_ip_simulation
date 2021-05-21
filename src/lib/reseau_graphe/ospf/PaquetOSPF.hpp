/**
 * @file    PaquetOSPF.hpp
 * @brief   Declaration de la classe abstraite PaquetOSPF.
 * @author  Gabriel Dos Santos
 * @date    Mai 2021
 */

#pragma once

#include <cstdint>

enum TypePaquet: uint8_t {
    Hello = 1, DBD, LSR, LSU, LSAck
};

class PaquetOSPF {
    protected:
        // Attributs
        TypePaquet m_Type;
        uint8_t m_IdRouteur;

    public:
        // Destructeur virtuel
        virtual ~PaquetOSPF() {}

        // Getters et setter
        virtual void setEntete(const TypePaquet& type, const uint8_t& idRouteur) = 0;
        virtual const TypePaquet& getType() const = 0;
        virtual const uint8_t& getIdRouteur() const = 0;
};
