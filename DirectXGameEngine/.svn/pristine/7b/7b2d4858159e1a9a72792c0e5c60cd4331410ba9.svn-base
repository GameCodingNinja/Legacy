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
      <SetColor r="1" g="0.43" b="0.43"/>
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

</commandScripts>
