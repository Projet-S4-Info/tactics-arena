#ifndef state_h
#define state_h
/** \file state.h
 * \brief Defining Statelist Structures and all primitive functions
 * \author Robin BUTEL
 */
#include "substruct.h"

/** \struct listelem
 * \brief Defined as List_Elem, is the basic structure for an element of a Statelist
 */
typedef struct listelem{
  Status * value;             //!< A pointer to a Status, keeps mod info, needed for removing the buff at the end of it's duration
  struct entity_t * entity;   //!< A pointer to the entity affected by the mod, needed for removing the buff at the end of it's duration
  struct listelem * suiv;     //!< A pointer to the next element of the list
  struct listelem * prec;     //!< A pointer to the precedent element of the list
} List_Elem;
/** \typedef List_Elem
 * \brief An element of a Statelist, containing mod info, modded entity, and list pointers
 */

/** \struct StateList
 * \brief A list, containing a flag and a pointer to the current element.
 */
typedef struct {
  List_Elem * drapeau; //!< A pointer to the flag element, acts as the list's root, allowing it to loop, while always knowing where the beginning and the end of the list is
  List_Elem * ec;      //!< A pointer to the current element of the list, if the list if empty, it will point to the flag
} StateList;

/** \fn init_list(StateList ** list)
 * \brief Initialises a list
 * \details Allocates dyanmically space for the flag and sets both pointers on it, as well as sets the flag's next and precedent pointers to it
 * \param list pointer to the list to initialise
 * \return err_t POINTER_NULL if malloc fails, otherwise err_t OK
 */
err_t init_list(StateList ** list);

/** \fn out_of_list(StateList * list)
 * \brief Indicates if current element is out of the list
 * \details Current element is not in the list if it points to the flag
 * \param list The list to check
 * \return TRUE if the current element (ec) is out of list
 */
bool out_of_list(StateList * list);

/** \fn list_empty(StateList * list)
 * \brief Indicates if a list is empty
 * \details A list is empty if the flag's next element and precedent element point to itself
 * \param list The list to check
 * \return TRUE if the list is empty
 */
bool list_empty(StateList * list);

/** \fn start_list(StateList * list)
 * \brief Sets the current element to the beginning of the list
 * \param list The list to set
 * \return an err_t custom error type
 */
err_t start_list(StateList * list);

/** \fn end_list(StateList * list)
 * \brief Sets the current element to the end of the list
 * \param list The list to set
 * \return err_t OK
 */
err_t end_list(StateList * list);

/** \fn list_next(StateList * list)
 * \brief Sets the current element to the next element of the list
 * \param list The list to set
 * \return err_t OK
 */
err_t list_next(StateList * list);

/** \fn list_change(StateList * list, int d)
 * \brief Increases or decreases current mod's duration
 * \details Changes current element's Status' duration by d (increase if positive, decrease if negative), if the duration falls to or under zero, then a pointer to the element is returned so that the mod may be removed
 * \param list The list to change
 * \param d How much to change the duration by (negative value if a decrease is wanted)
 * \return A pointer to the current element if the duration falls to or under zero, otherwise NULL
 */
List_Elem * list_change(StateList * list, int d);

/** \fn list_remove(StateList * list)
 * \brief Removes the current element from the list
 * \details frees all dynamically allocated pointers and sets their values to NULL to avoid any double frees
 * \param list The list to remove from
 * \return err_t OK
 */
err_t list_remove(StateList * list);

/** \fn list_add(StateList * list, Status v, struct entity_t * entity)
 * \brief Removes the current element from the list
 * \details frees all dynamically allocated pointers and sets their values to NULL to avoid any double frees
 * \param list The list to remove from
 * \return err_t OK
 */
err_t list_add(StateList * list, Status v, struct entity_t * entity);
List_Elem * list_search(StateList * list, struct entity_t * entity, statusId status); //Set status to -1 for any status
bool list_check(StateList * list);
err_t list_destroy(StateList * list);


#endif