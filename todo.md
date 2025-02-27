Current Todo List:
1. Order Creation Validation: think of extreme cases, such as creating a iceberg order, where the total quantity is less than what the user wants to display. Think of other extreme cases, and make sure that these are incorporated in the validation phase.
2. Automatic Order ID Generation - remove ID assignment from the constructor of classes - this should be done automatically 
3. In stop orders there is boolean value indicating whether the stop has been triggered or not. This should be automatically assigned. REMOVE from constructor 
4. In the iceberg order there is one additional field that shouldn't be there - hidden qty, is a value that should be calculated instead of given by the user - you have the totalQty which is inherited from the Order class, and the displayedQty which is assigned by the user 
5. Automatic Timestamp Generation: need to automatically assign the timestamp to an order, instead of allowing the user to manually assign it - this should be done in the Order class, and therefore the rest will inherit this property 
6. Fix the constructors everywhere in the cpp files - remove the timestamp parameter

DONE:
- Automatic Timestamp Generation
- Removed Timestamp from Order Constructor
- Removed ID from Order Constructor
