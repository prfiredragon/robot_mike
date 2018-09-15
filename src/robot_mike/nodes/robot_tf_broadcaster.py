#!/usr/bin/env python
import roslib
roslib.load_manifest('robot_mike')
import rospy

import tf
import robot_mike.msg

def handle_robot_pose(msg, robotname) :
    br = tf.Transform((msg.x, msg.y, 0),
                      tf.transformations.quaternion_from_euler(0, 0, msg.yheta),
                      rospy.Time.now(),
                      robotname,
                      "world")

if __name__ == '__main__' :
    rospy.init_node('robot_tf_broadcaster')
    robotname = rospy.get_param('~robot')
    rospy.Subscriber('/%s/pose' % robotname,
                     robot_mike.msg.Pose, 
                     handle_robot_pose,
                     robotname)
    rospy.spin()
