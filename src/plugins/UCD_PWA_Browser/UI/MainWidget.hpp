/*
 * @name UC Davis PI Web API Browser
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Browser for UC Davis PI Web API data.
 *
 * Data from OSIsoft and UC Davis
 * Icons and images owned by their respective owners
 */

#ifndef UCDPWAB_MAIN_WIDGET_H
#define UCDPWAB_MAIN_WIDGET_H

// Universal Include
#include "universalInclude.hpp"

/*
 * UCDPWAB = UC Davis PI Web API Browser
 */
namespace UCDPWAB {

class MainWidget : public QWidget {
  Q_OBJECT

public:
  MainWidget(){
      this->setObjectName("UC Davis PI Web API Browser Widget");
  };
  ~MainWidget(){};
};

} // namespace UCDPWAB

#endif
// end UCDPWAB_H
