<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetColor r="0.6" g="0" b="0" resetDefault="true"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetColor r="1" g="0" b="0"/>
      <ColorTo method="time" time="500" r="0.7"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="500" r="1"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="1" g="0.43" b="0.43"/>
    </scriptStep>

    <scriptStep>
      <SetHold hold="100"/>
    </scriptStep>

    <scriptStep>
      <SetColor r="1" g="0" b="0"/>
    </scriptStep>
  </command>

  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetColor r="0.5" g="0.5" b="0.5" resetDefault="true"/>
    </scriptStep>
  </command>


  <command name="transitionIn" endMode="stop">
    <scriptStep>
      <SetColor a="0"/>
      <ColorTo method="time" time="250" a="1"/>
    </scriptStep>
  </command>

  <command name="transitionOut" endMode="stop">
    <scriptStep>
      <SetColor a="1"/>
      <ColorTo method="time" time="250" a="0"/>
    </scriptStep>
  </command>

  <!-- Sample Scripts
  
  ATTRIBUTES
  NOTE: r, g, b, a=""..values range from 0.0 to 1.0
        type=""........is either absolute or relative
        method=""......is either speed or time. If
        speed="".......is the speed in pixels per second, or 0.001 unit of color per second
        time=""........is the number of seconds it takes to reach a point
  
  COLOR SCRIPTS
  <command name="something" fps="60" endMode="hide">
  
    <scriptStep>
      <SetColor r="1" g="1" b="1" a="1"/>
    </scriptStep>
    
    <scriptStep>
      <IncColor r="1" g="1" b="1" a="1"/>
    </scriptStep>
    
    <scriptStep>
      <ColorTo method="speed" speed="" r="1" g="1" b="1" a="1"/>
          or
      <ColorTo method="time" time="" r="1" g="1" b="1" a="1"/>
    </scriptStep>
    
    <scriptStep>
      <Color method="speed" speed="" s_r="" s_g="" s_b="" s_a=""  e_r="" e_g="" e_b="" e_a=""/>
          or
      <Color method="time" time="" s_r="" s_g="" s_b="" s_a=""  e_r="" e_g="" e_b="" e_a=""/>
    </scriptStep>
    
  </command>
  
  POSITION SCRIPTS
  <command name="something else" fps="60" endMode="hide">
  
    <scriptStep>
      <SetPosition x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <IncPosition x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <MoveTo method="speed" speed="" x="" y="" z=""/>
          or
      <MoveTo method="time" time="" x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <Move method="speed" speed="" x="" y="" z=""/>
          or
      <Move method="time" time="" x="" y="" z=""/>
    </scriptStep>
  
  </command>
  
  ROTATION SCRIPTS
  <command name="something else's something else" fps="60" endMode="hide">
  
    <scriptStep>
      <SetRotation x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <IncRotation x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <RotateTo method="speed" speed="" x="" y="" z=""/>
          or
      <RotateTo method="time" time="" x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <Rotate method="speed" speed="" x="" y="" z=""/>
          or
      <Rotate method="time" time="" x="" y="" z=""/>
    </scriptStep>
    
  </command>
  
  SCALE SCRIPTS
  <command name="adfadfasf" fps="60" endMode="hide">
  
    <scriptStep>
      <SetScale x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <IncScale x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <ScaleTo method="speed" speed="" x="" y="" z=""/>
          or
      <ScaleTo method="time" time="" x="" y="" z=""/>
    </scriptStep>
    
    <scriptStep>
      <Scale method="speed" speed="" x="" y="" z=""/>
          or
      <Scale method="time" time="" x="" y="" z=""/>
    </scriptStep>
    
  </command>
  -->
  
</commandScripts>
