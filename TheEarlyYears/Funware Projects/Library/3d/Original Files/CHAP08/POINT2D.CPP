//
// File name: Poin2D.CPP
//
// Description: Support file for the Point2D.HPP header file
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include "Point2D.HPP"

// Function designed to search through List in an attempt
// to determine uniqueness of vertex V:
int UniqueVert ( Point2D &V, Point2D *List, int Range )
  {
  // Loop through list of vertices:
  for ( int Count = 0; Count < Range; Count++ )
      {
      // If it's not unique, return false:
      if ( V == List [ Count ] )
         return 0;
      }
  // Return true (it's unique):
  return 1;
  }

// Function designed to search through List in an attempt
// to locate the index of a vertex that matches V:
unsigned int GetVertIndex ( Point2D &V, Point2D *List, unsigned int Range )
  {
  // Loop through the list of vertices:
  for ( unsigned int Count = 0; Count < Range; Count++ )
      {
      // If the vertex matches, return the index:
      if ( V == List [ Count ] )
         return Count;
      }
  // Return zero as default - Note: this code should
  // never be reached.
  return 0;
  }
