#ifdef _REFLECTIONLIST_H_
#define _REFLECTIONLIST_H_

#include "reflection.h"

using namespace std;

namespace hkl {

    template<typename T>
    class ReflectionList : MyVector<Reflection<T> >
    {
      unsigned int add(Reflection<T> const & reflection);
    }
    
    /**
     * @brief Add a reflection to the reflection liste
     * @param reflection the %Reflection to add.
     * @return the index of the added %Reflection.
     */
    template<typename T>
    unsigned int
    ReflectionList<T>::addReflection(Reflection<T> const & reflection) throw (HKLException)
      {
        // test the validity of the 
        if (fabs(reflection.get_geometry().get_source().get_waveLength()) < constant::math::epsilon_0)
            HKLEXCEPTION("Can not add a reflection with a null wave length.",
                         "Set the wave length properly");

        // If the reflection already exist put the flag to false
        if (reflection.get_flag())
          {
            typename vector<Reflection<T> >::iterator iter(m_reflectionList.begin());
            typename vector<Reflection<T> >::iterator end(m_reflectionList.end());
            while(iter != end)
              {
                if (fabs(reflection.get_h() - iter->get_h()) < constant::math::epsilon_0
                    && fabs(reflection.get_k() - iter->get_k()) < constant::math::epsilon_0
                    && fabs(reflection.get_l() - iter->get_l()) < constant::math::epsilon_0)
                  {
                    m_reflectionList.push_back(reflection);
                    m_reflectionList.back().set_flag(false);
                    return m_reflectionList.size();
                  }
                ++iter;
              }
          } 
        m_reflectionList.push_back(reflection);
        return m_reflectionList.size();
      }

    /**
     * @brief Delete the index reflection from the reflection list
     * @param index the index of the reflection to delete.
     */
    template<class T>
    void
    Crystal<T>::delReflection(unsigned int const & index) throw (HKLException)
      {
        unsigned int nb_reflection = m_reflectionList.size();

        if (index >= nb_reflection)
          {
            ostringstream reason;
            ostringstream description;

            reason << "Can not delete the reflection : " << index;
            if (nb_reflection)
                description << "For crystal \"" << get_name() << "\" the maximum index is : " << nb_reflection-1;
            else
                description << "The Crystal \"" << get_name() << "\" contain no reflection";

            HKLEXCEPTION(reason.str(), description.str());
          }
        else
          {
            typename vector<Reflection<T> >::iterator iter = m_reflectionList.begin();
            for(unsigned int i=0;i<index;i++)
                ++iter;
            m_reflectionList.erase(iter);
          }
      }

    /** 
     * @brief Delete all crystal's reflections.
     */
    template<typename T>
    void
    Crystal<T>::delAllReflections(void)
      {
        m_reflectionList.clear();
      }


    /** 
     * @brief Return the number of reflection in a crystal.
     * @return The number of reflection in the m_reflectionList vector.
     */
    template<typename T>
    unsigned int
    Crystal<T>::getReflectionListSize(void)
      {
        return m_reflectionList.size();
      }

    /**
     * @brief Modification of the ith reflections
     * @param index of the ith reflection to modify
     * @param reflection replace the ith reflection with that one.
     */
    /*
       template<class T>
       void
       Crystal<T>::setReflection(unsigned int const & index,
       Reflection<T> const & reflection) throw (HKLException)
       {
       unsigned int nb_reflection = m_reflectionList.size();

       if (index >= nb_reflection)
       {
       ostringstream reason;
       ostringstream description;

       reason << "The reflection number " << index << " is out of range";
       description << " you ask for the modification of the " << index 
       << "th reflection, but the cristal: " << get_name() << " containe only "
       << nb_reflection << " reflections";

       HKLEXCEPTION(reason.str(), description.str());
       }

    // If the reflection already exist put the flag to false      
    if (reflection.get_flag())
    {
    typename vector<Reflection<T> >::iterator iter(m_reflectionList.begin());
    typename vector<Reflection<T> >::iterator end(m_reflectionList.end());
    while(iter != end)
    {
    if (fabs(reflection.get_h() - iter->get_h()) < constant::math::epsilon_0
    && fabs(reflection.get_k() - iter->get_k()) < constant::math::epsilon_0
    && fabs(reflection.get_l() - iter->get_l()) < constant::math::epsilon_0)
    {
    m_reflectionList[index] = reflection;
    m_reflectionList[index].set_flag(false);
    return;
    }
    ++iter;
    }
    } 
    m_reflectionList[index] = reflection;
    }
    */
    /**
     * @brief Get a constant reference on a reflection.
     * @param index of the reflection.
     * @return A constant reference on the #m_reflectionList.
     */
    template<class T>
    Reflection<T> &
    Crystal<T>::getReflection(unsigned int const & index) throw (HKLException)
      {
        unsigned int nb_reflection = m_reflectionList.size();

        if (index >= nb_reflection)
          {
            ostringstream reason;
            ostringstream description;

            reason << "Index of the reflection is out of range : " << index;
            description << "Can not return a reference on a reflection for the Crystal \"" << get_name() << "\". The maximum index is : " << nb_reflection-1;

            HKLEXCEPTION(reason.str(), description.str());
          }
        else
            return m_reflectionList[index];
      }

    /**
     * @brief Get a constant reference on a reflection.
     * @param index of the reflection.
     * @return A constant reference on the #m_reflectionList.
     */
    template<class T>
    Reflection<T> const &
    Crystal<T>::getReflection(unsigned int const & index) const throw (HKLException)
      {
        unsigned int nb_reflection = m_reflectionList.size();

        if (index >= nb_reflection)
          {
            ostringstream reason;
            ostringstream description;

            reason << "Index of the reflection is out of range : " << index;
            description << "Can not return a constant reference on the reflection numbered " << index << " for the Crystal \"" << get_name() << "\". The maximum index is : " << nb_reflection-1;

            HKLEXCEPTION(reason.str(), description.str());
          }
        else
            return m_reflectionList[index];
      }

    /**
     * @brief Return true or false if the crystal contain at least nb_reflections independant reflections.
     * @param nb_reflections the minimim number of independant reflections.
     * @return true if crystal contain at least nb_reflections independant reflections. false otherwise.
     *
     * We comptabilize Colinear reflections as one unique reflection available for computation.
     * (ex (1,0,0) == (2,0,0)).
     */
    template<class T>
    bool
    Crystal<T>::isEnoughReflections(unsigned int nb_reflections) const
      {
        unsigned int nb_usable_reflections = 0;
        typename vector<Reflection<T> >::const_iterator iter = m_reflectionList.begin();
        typename vector<Reflection<T> >::const_iterator iter2 = m_reflectionList.begin();
        typename vector<Reflection<T> >::const_iterator end = m_reflectionList.end();

        while(iter < end && nb_usable_reflections < nb_reflections)
          {
            if (iter->get_flag())
              {
                if (nb_usable_reflections == 0)
                    nb_usable_reflections = 1;
                iter2 = iter;
                ++iter2;
                while(iter2 < end && nb_usable_reflections < nb_reflections)
                  {
                    if (iter2->get_flag() && !iter->isColinear(*iter2))
                        nb_usable_reflections++;
                    ++iter2;
                  }
              }
            ++iter;
          }
        if (nb_usable_reflections == nb_reflections)
            return true;
        else
            return false;
      }

    /**
     * @brief Compute the orientation matrix from two basic non-parallel reflections.
     *
     * Compute the orientation matrix from two basic non-parallel reflections.
     */
    template<class T>
    void
    Crystal<T>::computeU(void) throw (HKLException)
      {
        if (!isEnoughReflections(2))
          {
            ostringstream reason;
            reason << "Can not compute the U matrix of the Crystal \"" << get_name() << "\" with less than 2 active reflections";
            HKLEXCEPTION(reason.str(),
                         "Please set at least 2 active reflections.");
          }
        else
          {
            typename vector<Reflection<T> >::iterator iter = m_reflectionList.begin();
            iter = _getNextReflectionIteratorForCalculation(iter);
            svector h1c = m_B * iter->getHKL();
            svector u1phi = iter->get_hkl_phi();
            if (u1phi == svector())
              {
                ostringstream reason;
                ostringstream description;
                reason << "Can not compute U with this " << h1c << " reflection.";
                if (iter->get_geometry().get_source().getKi() == svector())
                    description << "The Ki vector is null check the source parameters.";
                else
                    description << "You are looking to the direct beam, find a diffraction peak first ;)";
                HKLEXCEPTION(reason.str(), description.str());
              }

            ++iter;
            iter = _getNextReflectionIteratorForCalculation(iter);
            svector h2c = m_B * iter->getHKL();
            svector u2phi = iter->get_hkl_phi();
            if (u2phi == svector())
              {
                ostringstream reason;
                ostringstream description;
                reason << "Can not compute U with this " << h2c << " reflection.";
                if (iter->get_geometry().get_source().getKi() == svector())
                    description << "The Ki vector is null check the source parameters.";
                else
                    description << "You are looking to the direct beam, find a diffraction peak first ;)";
                HKLEXCEPTION(reason.str(), description.str());
              }

            // Compute matrix Tc from h1c and h2c.
            smatrix Tc = h1c.axisSystem(h2c).transpose();

            // Compute Tphi.
            smatrix Tphi = u1phi.axisSystem(u2phi);

            // Compute U from equation (27).
            m_U = Tphi;
            m_U *= Tc;
          }
      }

    /**
     * @brief Compute the leastSquare of the crystal.
     * @return the variance.
     */
    template<class T>
    double
    Crystal<T>::fitness(void) throw (HKLException)
      {
        unsigned int nb_reflections = 0;
        double fitness = 0.;
        svector hkl_phi, hkl_phi_c;

        if (!isEnoughReflections(1))
          {
            ostringstream reason;
            reason << "Can not compute the fitness of the Crystal \"" << get_name() << "\" with less than 1 active reflection.";
            HKLEXCEPTION(reason.str(),
                         "Please set at least 1 active reflections.");
          }
        else
          {
            _computeB();
            _computeU();
            typename vector<Reflection<T> >::const_iterator iter = m_reflectionList.begin();
            typename vector<Reflection<T> >::const_iterator end = m_reflectionList.end();
            while(iter != end)
              {
                if (iter->get_flag())
                  {
                    hkl_phi = iter->get_hkl_phi();
                    hkl_phi_c = m_U * m_B * iter->getHKL();
                    hkl_phi -= hkl_phi_c;
                    fitness += hkl_phi[0]*hkl_phi[0] + hkl_phi[1]*hkl_phi[1] + hkl_phi[2]*hkl_phi[2];
                    nb_reflections++;
                  }
                ++iter;
              }
            fitness /= 3*nb_reflections;

            return fitness;
          }
      }

    /**
     * @brief Randomize the crystal
     */
    template<class T>
    void
    Crystal<T>::randomize(void)
      {
        FitParameterList::randomize();
        svector a, b, c;
        svector axe;

        // La valeur des angles alpha, beta et gamma ne sont pas indépendant.
        // Il faut donc gérer les différents cas.

        unsigned int angles_to_fit = _alpha->get_flagFit() + _beta->get_flagFit() + _gamma->get_flagFit();

        switch (angles_to_fit)
          {
          case 0:
            break;
          case 1:
            if (_alpha->get_flagFit()) // alpha
              {
                a.set(1, 0, 0);
                b = a.rotatedAroundVector(axe.randomize(a), _gamma->get_current().get_value());
                c = a.rotatedAroundVector(axe.randomize(a), _beta->get_current().get_value());
                _alpha->set_current(b.angle(c));
              }
            else if (_beta->get_flagFit())
              { // beta
                a.set(1, 0, 0);
                b = a.rotatedAroundVector(axe.randomize(a), _gamma->get_current().get_value());
                c = b.rotatedAroundVector(axe.randomize(b), _alpha->get_current().get_value());
                _beta->set_current(a.angle(c));
              }
            else
              { // gamma
                a.set(1, 0, 0);
                c = a.rotatedAroundVector(axe.randomize(a), _beta->get_current().get_value());
                b = c.rotatedAroundVector(axe.randomize(c), _alpha->get_current().get_value());
                _gamma->set_current(a.angle(b));
              }
            break;
          case 2:
            if (_alpha->get_flagFit())
              {
                if (_beta->get_flagFit()) // alpha + beta
                  {
                    a.set(1, 0, 0);
                    b = a.rotatedAroundVector(axe.randomize(a), _gamma->get_current().get_value());
                    c.randomize(a, b);
                    _alpha->set_current(b.angle(c));
                    _beta->set_current(a.angle(c));
                  }
                else
                  { // alpha + gamma
                    a.set(1, 0, 0);
                    c = a.rotatedAroundVector(axe.randomize(a), _beta->get_current().get_value());
                    b.randomize(a, c);
                    _alpha->set_current(b.angle(c));
                    _gamma->set_current(a.angle(b));
                  }
              } 
            else
              { // beta + gamma
                b.set(1, 0, 0);
                c = b.rotatedAroundVector(axe.randomize(b), _alpha->get_current().get_value());
                a.randomize(b, c);
                _beta->set_current(a.angle(c));
                _gamma->set_current(a.angle(b));
              }
            break;
          case 3:
            a.randomize();
            b.randomize(a);
            c.randomize(a, b);
            _alpha->set_current(b.angle(c));
            _beta->set_current(a.angle(c));
            _gamma->set_current(a.angle(b));
            break;
          }
        _computeB();
        _computeU();
      }

    /**
     * @brief overload of the == operator for the cristal class
     * @param C The crystal we want to compare.
     */
    template<class T>
    bool
    Crystal<T>::operator == (Crystal const & crystal) const
      {
        return FitParameterList::operator==(crystal)
        && Object::operator==(crystal)
        && m_B == crystal.m_B
        && m_U == crystal.m_U
        && m_reflectionList == crystal.m_reflectionList;
      }

    /**
     * @brief Print the state of the current crystal on a ostream.
     * @param flux the ostream to write into.
     * @return the flux modified.
     */
    template<class T>
    ostream &
    Crystal<T>::printToStream(ostream & flux) const
      { 
        unsigned int i;
        unsigned int j;

        // Parameters
        flux << "\"" << get_name() << "\"" << endl;
        flux.width(9); flux << "  Parameters:";
        flux.width(9); flux << "value";
        flux.width(9); flux << "min";
        flux.width(9); flux << "max";
        flux << endl;
        for(i=0;i<3;i++)
          {
            FitParameter const & p = (*this).at(i);
            flux.precision(3);
            flux.width(9); flux << p.get_name() << "(" << p.get_flagFit() << "):";
            flux.width(9); flux << p.get_current();
            flux.width(9); flux << p.get_min();
            flux.width(9); flux << p.get_max();
            flux << endl;
          }
        for(i=3;i<6;i++)
          {
            FitParameter const & p = (*this).at(i);
            flux.precision(3);
            flux.width(9); flux << p.get_name() << "(" << p.get_flagFit() << "):";
            flux.width(9); flux << p.get_current()*constant::math::radToDeg;
            flux.width(9); flux << p.get_min()*constant::math::radToDeg;
            flux.width(9); flux << p.get_max()*constant::math::radToDeg;
            flux << endl;
          }
        flux << endl << "  UB:" << endl;
        smatrix UB = get_U() * get_B();
        flux.precision(3);
        for(i=0;i<3;i++)
          {
            flux << "  ";
            for(j=0;j<3;j++)
              {
                flux.width(9);
                flux << UB.get(i,j);
              }
            flux << endl;
          }

        //Reflections
        if (m_reflectionList.size())
          {
            flux << endl << "  Reflections:" << endl
            << "  n";
            flux.width(9); flux << "h";
            flux.width(9); flux << "k";
            flux.width(9); flux << "l";
            flux << "  ";
            vector<MyString> axesNames = m_reflectionList[0].get_geometry().getAxesNames();
            unsigned int n = axesNames.size();
            for(i=0;i<n;i++)
              {
                flux.width(9);
                flux << axesNames[i];
              }
            flux << "  ";
            flux.width(9); flux << "lambda";
            flux << endl;
            typename vector<Reflection<T> >::const_iterator iter = m_reflectionList.begin();
            typename vector<Reflection<T> >::const_iterator end = m_reflectionList.end();
            n = 1;
            while(iter != end)
              {
                flux << "  " << n << *iter << endl;
                ++iter;
                ++n;
              }
          }
        else
            flux << endl << "  No reflection" << endl;
        return flux;
      }

    template<class T>
    ostream &
    Crystal<T>::toStream(ostream & flux) const
      {
        Object::toStream(flux);
        FitParameterList::toStream(flux);
        m_B.toStream(flux);
        m_U.toStream(flux);

        unsigned int nb_reflections = m_reflectionList.size();
        flux << " " << nb_reflections << endl;
        typename vector<Reflection<T> >::const_iterator iter = m_reflectionList.begin();
        typename vector<Reflection<T> >::const_iterator end = m_reflectionList.end();
        while(iter != end)
          {
            iter->toStream(flux);
            ++iter;
          }

        return flux;
      }

    /**
     * \brief Restore a Crystal from a stream.
     * \param flux The stream containing the Crystal.
     */
    template<class T>
    istream &
    Crystal<T>::fromStream(istream & flux)
      {
        Object::fromStream(flux);
        FitParameterList::fromStream(flux);
        m_B.fromStream(flux);
        m_U.fromStream(flux);

        unsigned int nb_reflections;
        unsigned int i;
        flux >> nb_reflections;
        for(i=0;i<nb_reflections;i++)
          {
            m_reflectionList.push_back(Reflection<T>());
            m_reflectionList[i].fromStream(flux);
          }
        //_computeB();
        return flux;
      }


    /**
     * @brief The main function to compute the matrix B.
     *
     * This method computes the B matrix from the cristal parameters.
     * It is the transformation matrix from the reciprocal base to an orthonormal
     * one.
     * @f[
     *  B = \left(
     *    \begin{matrix}
     *      b_1 & b_2\cos\beta_3  & b_3\cos\beta_2 \\
     *      0   & b_2\sin\beta_3  & -b_3\sin\beta_2\cos\alpha_1 \\
     *      0   & 0               & 1/a_3
     *    \end{matrix}
     *  \right)
     * @f]
     */
    template<class T>
    void
    Crystal<T>::_computeB(void)
      {
        double a_star, b_star, c_star, alpha_star, beta_star, gamma_star;
        getReciprocalLattice(&a_star, &b_star, &c_star, &alpha_star, &beta_star, &gamma_star);

        double c = (*this)["c"].get_value();

        m_B.set( a_star, b_star * cos(gamma_star),                   c_star * cos(beta_star),
                 0.    , b_star * sin(gamma_star), c_star * sin(beta_star) * cos(alpha_star),
                 0.    ,                       0.,                 constant::physic::tau / c);
      }

    /*!
     * \brief The main function to compute the U matrix.
     *
     * This method compute U using the 3 eulerian angles.
     * in fact euler_x, euler_y and euler_z parameters.
     */
    template<class T>
    void
    Crystal<T>::_computeU(void)
      {
        double euler_x = (*this)["euler_x"].get_value();
        double euler_y = (*this)["euler_y"].get_value();
        double euler_z = (*this)["euler_z"].get_value();

        set_U(smatrix(euler_x, euler_y, euler_z));
      }

    /**
     * @brief Return the index of the next usable reflection for calculation
     * @param from The iterator of the reflection from which the search start.
     * @return The iterator of the next reflection valid for calculus.
     */
    template<class T>
    typename vector<Reflection<T> >::iterator &
    Crystal<T>::_getNextReflectionIteratorForCalculation(typename vector<Reflection<T> >::iterator & from) throw (HKLException)
      {
        typename vector<Reflection<T> >::iterator end = m_reflectionList.end();

        while(from < end)
          {
            if (from->get_flag())
                return from;
            ++from;
          }
        HKLEXCEPTION("No more reflection.",
                     "Please add reflections.");
      }

} // namespace hkl

#endif // _REFLECTIONLIST_H_