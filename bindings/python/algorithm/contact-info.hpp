//
// Copyright (c) 2019-2020 INRIA
//

#ifndef __pinocchio_python_algorithm_contact_info_hpp__
#define __pinocchio_python_algorithm_contact_info_hpp__

#include <eigenpy/memory.hpp>

#include "pinocchio/algorithm/contact-info.hpp"
#include "pinocchio/bindings/python/utils/macros.hpp"
#include "pinocchio/bindings/python/utils/comparable.hpp"

EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(pinocchio::python::context::RigidContactModel)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(pinocchio::python::context::RigidContactData)

namespace pinocchio
{
  namespace python
  {
    namespace bp = boost::python;
  
    template<typename BaumgarteCorrectorParameters>
    struct BaumgarteCorrectorParametersPythonVisitor
    : public boost::python::def_visitor< BaumgarteCorrectorParametersPythonVisitor<BaumgarteCorrectorParameters> >
    {
      typedef typename BaumgarteCorrectorParameters::Scalar Scalar;
      typedef BaumgarteCorrectorParameters Self;
      
    public:
      
      template<class PyClass>
      void visit(PyClass& cl) const
      {
        cl
        .def(bp::init<>(bp::arg("self"),
                        "Default constructor."))
        
        .PINOCCHIO_ADD_PROPERTY(Self,Kp,"Proportional corrector value.")
        .PINOCCHIO_ADD_PROPERTY(Self,Kd,"Damping corrector value.")
        .def(ComparableVisitor<Self,pinocchio::is_floating_point<Scalar>::value>())
        ;
      }
      
      static void expose()
      {
        bp::class_<BaumgarteCorrectorParameters>("BaumgarteCorrectorParameters",
                                                 "Paramaters of the Baumgarte Corrector.",
                                                 bp::no_init)
        .def(BaumgarteCorrectorParametersPythonVisitor())
        ;
        
      }
    };
    
    template<typename RigidContactModel>
    struct RigidContactModelPythonVisitor
    : public boost::python::def_visitor< RigidContactModelPythonVisitor<RigidContactModel> >
    {
      typedef typename RigidContactModel::Scalar Scalar;
      typedef typename RigidContactModel::SE3 SE3;
      typedef RigidContactModel Self;
      typedef typename RigidContactModel::ContactData ContactData;
      typedef typename RigidContactModel::BaumgarteCorrectorParameters BaumgarteCorrectorParameters;

    public:
      
      template<class PyClass>
      void visit(PyClass& cl) const
      {
        cl
        .def(bp::init<>(bp::arg("self"),
                        "Default constructor."))
        .def(bp::init<ContactType,JointIndex,SE3,JointIndex,SE3,bp::optional<ReferenceFrame> >
             ((bp::arg("self"),
               bp::arg("contact_type"),
               bp::arg("joint1_id"),
               bp::arg("joint1_placement"),
               bp::arg("joint2_id"),
               bp::arg("joint2_placement"),
               bp::arg("reference_frame")),
              "Contructor from a given ContactType, joint index and placement for the two joints implied in the constraint."))
        .def(bp::init<ContactType,JointIndex,SE3,bp::optional<ReferenceFrame> >
             ((bp::arg("self"),
               bp::arg("contact_type"),
               bp::arg("joint1_id"),
               bp::arg("joint1_placement"),
               bp::arg("reference_frame")),
              "Contructor from a given ContactType, joint index and placement only for the first joint implied in the constraint."))
        .def(bp::init<ContactType,JointIndex,bp::optional<ReferenceFrame> >
             ((bp::arg("self"),
               bp::arg("contact_type"),
               bp::arg("joint1_id"),
               bp::arg("reference_frame")),
              "Contructor from a given ContactType and joint index. The base joint is taken as 0 in the constraint."))        
        .PINOCCHIO_ADD_PROPERTY(Self,name,
                                "Name of the contact.")
        .PINOCCHIO_ADD_PROPERTY(Self,type,
                                "Type of the contact.")
        .PINOCCHIO_ADD_PROPERTY(Self,joint1_id,
                                "Index of first parent joint in the model tree.")
        .PINOCCHIO_ADD_PROPERTY(Self,joint2_id,
                                "Index of second parent joint in the model tree.")
        .PINOCCHIO_ADD_PROPERTY(Self,joint1_placement,
                                "Relative placement with respect to the frame of joint1.")
        .PINOCCHIO_ADD_PROPERTY(Self,joint2_placement,
                                "Relative placement with respect to the frame of joint2.")
        .PINOCCHIO_ADD_PROPERTY(Self,reference_frame,"Reference frame where the constraint is expressed (WORLD, LOCAL_WORLD_ALIGNED or LOCAL).")
        .PINOCCHIO_ADD_PROPERTY(Self,desired_contact_placement,"Desired contact placement.")
        .PINOCCHIO_ADD_PROPERTY(Self,desired_contact_velocity,"Desired contact spatial velocity.")
        .PINOCCHIO_ADD_PROPERTY(Self,desired_contact_acceleration,"Desired contact spatial acceleration.")
        .PINOCCHIO_ADD_PROPERTY(Self,corrector,"Corrector parameters.")
        
        .def("size", &RigidContactModel::size, "Size of the contact")
        
        .def("createData",
             &RigidContactModelPythonVisitor::createData,
             "Create a Data object for the given model.")
        
        .def(ComparableVisitor<Self,pinocchio::is_floating_point<Scalar>::value>())
        ;
      }
      
      static void expose()
      {
        bp::class_<RigidContactModel>("RigidContactModel",
                                      "Rigid contact model for contact dynamic algorithms.",
                                      bp::no_init)
        .def(RigidContactModelPythonVisitor())
        ;
        
        BaumgarteCorrectorParametersPythonVisitor<BaumgarteCorrectorParameters>::expose();
        
      }
      
      static ContactData createData(const Self & self)
      {
        return ContactData(self);
      }
    };
  
    template<typename RigidContactData>
    struct RigidContactDataPythonVisitor
    : public boost::python::def_visitor< RigidContactDataPythonVisitor<RigidContactData> >
    {
      typedef typename RigidContactData::Scalar Scalar;
      typedef RigidContactData Self;
      typedef typename RigidContactData::ContactModel ContactModel;

    public:
      
      template<class PyClass>
      void visit(PyClass& cl) const
      {
        cl
        .def(bp::init<ContactModel>(bp::args("self","contact_model"),
                                    "Default constructor."))
        
        .PINOCCHIO_ADD_PROPERTY(Self,contact_force,
                                "Constraint force.")
        .PINOCCHIO_ADD_PROPERTY(Self,oMc1,
                                "Placement of the constraint frame 1 with respect to the WORLD frame.")
        .PINOCCHIO_ADD_PROPERTY(Self,oMc2,
                                "Placement of the constraint frame 2 with respect to the WORLD frame.")
        .PINOCCHIO_ADD_PROPERTY(Self,c1Mc2,
                                "Relative displacement between the two frames.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact_placement_error,
                                "Current contact placement error between the two contact Frames.\n"
                                "This corresponds to the relative placement between the two contact Frames seen as a Motion error.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact1_velocity,
                                "Current contact Spatial velocity of the constraint 1.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact2_velocity,
                                "Current contact Spatial velocity of the constraint 2.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact_velocity_error,
                                "Current contact Spatial velocity error between the two contact Frames.\n"
                                "This corresponds to the relative velocity between the two contact Frames.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact_acceleration,
                                "Current contact Spatial acceleration.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact_acceleration_desired,
                                "Desired contact acceleration.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact_acceleration_error,
                                "Current contact spatial error (due to the integration step).")
        .PINOCCHIO_ADD_PROPERTY(Self,contact1_acceleration_drift,
                                "Current contact drift acceleration (acceleration only due to the Coriolis and centrifugal effects) of the contact frame 1.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact2_acceleration_drift,
                                "Current contact drift acceleration (acceleration only due to the Coriolis and centrifugal effects) of the contact frame 2.")
        .PINOCCHIO_ADD_PROPERTY(Self,contact_acceleration_deviation,
                                "Contact deviation from the reference acceleration (a.k.a the error).")
        
        .def(ComparableVisitor<Self,pinocchio::is_floating_point<Scalar>::value>())
        ;
      }
      
      static void expose()
      {
        bp::class_<RigidContactData>("RigidContactData",
                                     "Rigid contact data associated to a RigidContactModel for contact dynamic algorithms.",
                                     bp::no_init)
        .def(RigidContactDataPythonVisitor())
        ;
        
      }
    };
    
  } // namespace python
} // namespace pinocchio

#endif // ifndef __pinocchio_python_algorithm_contact_info_hpp__
