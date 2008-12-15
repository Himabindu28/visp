/****************************************************************************
 *
 * $Id: vpRobotAfma4.h,v 1.9 2008-12-15 17:19:22 fspindle Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Interface for the Irisa's Afma4 robot.
 *
 * Authors:
 * Fabien Spindler
 *
 *****************************************************************************/

#ifndef vpRobotAfma6_h
#define vpRobotAfma6_h

#include <visp/vpConfig.h>

#ifdef VISP_HAVE_AFMA4

#include <iostream>
#include <stdio.h>

#include <visp/vpRobot.h>
#include <visp/vpColVector.h>
#include <visp/vpDebug.h>
#include <visp/vpAfma4.h>

// low level controller api
extern "C" {
#  include "irisa_Afma4.h"
#  include "trycatch.h"
}


/*!
  \class vpRobotAfma4

  \ingroup Afma4

  \brief Control of Irisa's cylindrical robot named Afma4.

  Implementation of the vpRobot class in order to control Irisa's Afma4 robot.
  This robot is a cylindrical robot with four degrees of freedom manufactured
  in 1995 by the french Afma-Robots company. In 2008, the low level controller
  change for a more recent Adept technology based on the MotionBlox
  controller. A firewire camera is mounted on the end-effector to allow
  eye-in-hand visual servoing. The control of this camera is achieved by the
  vp1394TwoGrabber class. A Servolens lens is attached to the camera. It allows
  to control the focal lens, the iris and the focus throw a serial link. The
  control of the lens is possible using the vpServolens class.

  This class allows to control the Afma4 cylindrical robot in position
  and velocity:
  - in the joint space (vpRobot::ARTICULAR_FRAME), 
  - in the fixed reference frame (vpRobot::REFERENCE_FRAME), 
  - in the camera frame (vpRobot::CAMERA_FRAME),
 
  Mixed frame (vpRobot::MIXT_FRAME) where translations are expressed 
  in the reference frame and rotations in the camera frame is not implemented.

  All the translations are expressed in meters for positions and m/s
  for the velocities. Rotations are expressed in radians for the
  positions, and rad/s for the rotation velocities.

  The direct and inverse kinematics models are implemented in the
  vpAfma4 class.

  \warning A Ctrl-C, a segmentation fault or other system errors are
  catched by this class to stop the robot.

  To communicate with the robot, you may first create an instance of this
  class by calling the default constructor:

  \code
  vpRobotAfma4 robot;
  \endcode

  This initialize the robot kinematics with the eMc extrinsic camera
  parameters.

  To control the robot in position, you may set the controller
  to position control and than send the position to reach in a specific
  frame like here in the joint space:

  \code
  vpColVector q(4);
  // Set a joint position
  q[0] =  M_PI/2; // X axis, in radian
  q[1] =  0.2;    // Y axis, in meter
  q[2] = -M_PI/2; // A axis, in radian
  q[3] =  M_PI/8; // B axis, in radian

  // Initialize the controller to position control
  robot.setRobotState(vpRobot::STATE_POSITION_CONTROL);

  // Moves the robot in the joint space
  robot.setPosition(vpRobot::ARTICULAR_FRAME, q);
  \endcode

  The robot moves to the specified position with the default
  positioning velocity vpRobotAfma4::defaultPositioningVelocity. The
  setPositioningVelocity() method allows to change the maximal
  velocity used to reach the desired position.

  \code
  // Set the max velocity to 40%
  robot.setPositioningVelocity(40);

  // Moves the robot in the joint space
  robot.setPosition(vpRobot::ARTICULAR_FRAME, q);
  \endcode

  To control the robot in velocity, you may set the controller to
  velocity control and than send the velocities. To end the velocity
  control and stop the robot you have to set the controller to the
  stop state. Here is an example of a velocity control in the joint
  space:

  \code
  vpColVector qvel(6);
  // Set a joint velocity
  qvel[0] = M_PI/8; // X axis, in rad/s
  qvel[1] = 0.2;    // Y axis, in m/s
  qvel[2] = 0;      // A axis, in rad/s
  qvel[3] = M_PI/8; // B axis, in rad/s

  // Initialize the controller to position control
  robot.setRobotState(vpRobot::STATE_VELOCITY_CONTROL);

  while (...) {
    // Apply a velocity in the joint space
    robot.setVelocity(vpRobot::ARTICULAR_FRAME, qvel);

    // Compute new velocities qvel...
  }

  // Stop the robot
  robot.setRobotState(vpRobot::STATE_STOP)
  \endcode

  There is also possible to measure the robot current position with
  getPosition() method and the robot current velocities with the getVelocity()
  method.

  For convenience, there is also the ability to read/write joint
  positions from a position file with readPosFile() and writePosFile()
  methods.
*/
class VISP_EXPORT vpRobotAfma4
  :
  public vpAfma4,
  public vpRobot
{

private: /* Not allowed functions. */

  /*!
    Copy contructor not allowed.
   */
  vpRobotAfma4 (const vpRobotAfma4 & robot);

private: /* Attributs prives. */

  /** \brief Vrai ssi aucun objet de la classe vpRobotAfma4 n'existe.
   *
   * Il ne peut exister simultanement qu'un seul objet de la classe
   * vpRobotAfma4, car il correspond a un seul robot AFMA4. Creer
   * simultanement deux objets peut engendrer des conflits. Le constructeur
   * lance une erreur si le champ n'est pas FAUX puis positionne le champ
   * a VRAI. Seul le destructeur repositionne le champ a FAUX, ce qui
   * alors la creation d'un nouvel objet.
   */
  static bool robotAlreadyCreated;

  double positioningVelocity;

  // Variables used to compute the measured velocities (see getVelocity() )
  vpColVector q_prev_getvel;
  vpHomogeneousMatrix fMc_prev_getvel;
  double time_prev_getvel;
  bool first_time_getvel;

  // Variables used to compute the measured displacement (see
  // getDisplacement() )
  vpColVector q_prev_getdis;
  bool first_time_getdis;

public:  /* Constantes */

  /* Vitesse maximale par default lors du positionnement du robot.
   * C'est la valeur a la construction de l'attribut prive \a
   * positioningVelocity. Cette valeur peut etre changee par la fonction
   * #setPositioningVelocity.
   */
  static const double defaultPositioningVelocity; // = 20.0;

public:  /* Methode publiques */

  vpRobotAfma4 (void);
  virtual ~vpRobotAfma4 (void);

  void init (void);

  /* --- ETAT ------------------------------------------------------------- */

  vpRobot::vpRobotStateType setRobotState (vpRobot::vpRobotStateType newState);

  /* --- POSITIONNEMENT --------------------------------------------------- */
  void setPosition(const vpRobot::vpControlFrameType frame,
		   const vpColVector &position) ;
  void setPosition (const vpRobot::vpControlFrameType frame,
		    const double q1, const double q2,
		    const double q3, const double q4) ;
  void setPosition(const char *filename) ;
  void setPositioningVelocity (const double velocity);

  void getPosition (const vpRobot::vpControlFrameType frame,
		    vpColVector &position);

  double getPositioningVelocity (void);

  /* --- VITESSE ---------------------------------------------------------- */

  void setVelocity (const vpRobot::vpControlFrameType frame,
		    const vpColVector & velocity);


  void getVelocity (const vpRobot::vpControlFrameType frame,
		    vpColVector & velocity);

  vpColVector getVelocity (const vpRobot::vpControlFrameType frame);

  void get_cMe(vpHomogeneousMatrix &_cMe) ;
  void get_cVe(vpTwistMatrix &_cVe) ;
  void get_eJe(vpMatrix &_eJe)  ;
  void get_fJe(vpMatrix &_fJe)  ;

  void stopMotion() ;
  void powerOn() ;
  void powerOff() ;
  bool getPowerState();

  void move(const char *filename) ;
  static bool readPosFile(const char *filename, vpColVector &q)  ;
  static bool savePosFile(const char *filename, const vpColVector &q)  ;

  void getCameraDisplacement(vpColVector &displacement);
  void getArticularDisplacement(vpColVector &displacement);
  void getDisplacement(vpRobot::vpControlFrameType frame, 
		       vpColVector &displacement);

};


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */

#endif
#endif /* #ifndef vpRobotAfma4_h */
