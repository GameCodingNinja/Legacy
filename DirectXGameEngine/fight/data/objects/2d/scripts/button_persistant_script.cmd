<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetColor r="0.25" g="0.49" b=".75" resetDefault="true"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetColor r="0.30" g="0.52" b="0.77"/>
      <ColorTo method="time" time="500" r="0.15" g="0.29" b=".46"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="500" r="0.30" g="0.52" b="0.77"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetColor r="0.53" g="0.78" b="1"/>
      <ColorTo method="time" time="120" r="0.15" g="0.29" b=".46"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="120" r="0.53" g="0.78" b="1"/>
    </scriptStep>

    <scriptStep>
      <SetColor r="0.12" g="0.25" b="0.4"/>
    </scriptStep>
  </command>

  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetColor r="0.5" g="0.5" b="0.5" resetDefault="true"/>
    </scriptStep>
  </command>

  <command name="active_conflicted" endMode="stop">
    <scriptStep>
      <SetColor r=".8" g="0.20" b="1" resetDefault="true"/>
    </scriptStep>
  </command>

  <command name="inactive_conflicted" endMode="stop">
    <scriptStep>
      <SetColor r=".8" g="0.20" b="1" resetDefault="true"/>
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
