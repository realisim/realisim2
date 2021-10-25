This code is higly inspired from

https://github.com/preshing/FlexibleReflection

The main difference is as follow.
We wanted to be able to have something akind to a visitor pattern to visit reflected class/members.

The reason for that is to enable the end user to create any kind of serialization without modifying the
Reflection code.

With the code here, we are able to inherit IReflectableVisitor and simply by reimplementing the virtual interface,
a custom serializer can be created.