
# Programming project

## Group elements

- Filipe Silva Pinto Lopes
- João Rodrigues Vila Cova
- Ana Frederica Ferreira e Pereira


## Accomplished tasks

Implemented the element classes respecting their hierarchies.
Defined the transform function for each element.
Implemented the group attribute creating a new class and using a recursive approach in the reading logic.
To handle with `<use>`, we utilized an unordered map where the keys are the IDs and the values are the dynamically allocated SVGElement objects.

## Quality of life and performance improvements

To improve performance, an unordered_map (lookup efficiency: O(1)) was used instead of a map (lookup efficiency: O(log n)) in the implementation of the `<use>` element.

In ReadSVG.cpp, the parameters "transform," "origin," and "id" are of type const char* (cstring) instead of string, like "elementType." This was done so that a null pointer could be used in if statements to detect whether the parameters exist and hence influence the original element. This was not done with "elementType" because std::string allows for easier readability when comparing strings in if statements.
