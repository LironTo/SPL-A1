#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        SelectionPolicy();
        SelectionPolicy(const SelectionPolicy &other);
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
        SelectionPolicy &operator=(const SelectionPolicy &other) = delete;
        SelectionPolicy(SelectionPolicy &&other) = delete;
        SelectionPolicy &operator=(SelectionPolicy &&other) = delete;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        NaiveSelection(const NaiveSelection &other);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection *clone() const override;
        ~NaiveSelection() override = default;
        NaiveSelection &operator=(const NaiveSelection &other);
        NaiveSelection(NaiveSelection &&other);
        NaiveSelection &operator=(NaiveSelection &&other);

    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        BalancedSelection(const BalancedSelection &other);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
        void setScores(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        BalancedSelection &operator=(const BalancedSelection &other);
        BalancedSelection(BalancedSelection &&other);
        BalancedSelection &operator=(BalancedSelection &&other);

    private:
        int distance(const FacilityType &facility) const;
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        EconomySelection(const EconomySelection &other);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;
        EconomySelection &operator=(const EconomySelection &other);
        EconomySelection(EconomySelection &&other);
        EconomySelection &operator=(EconomySelection &&other);

    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        SustainabilitySelection(const SustainabilitySelection &other);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() override = default;
        SustainabilitySelection &operator=(const SustainabilitySelection &other);
        SustainabilitySelection(SustainabilitySelection &&other);
        SustainabilitySelection &operator=(SustainabilitySelection &&other);
        
    private:
        int lastSelectedIndex;
};