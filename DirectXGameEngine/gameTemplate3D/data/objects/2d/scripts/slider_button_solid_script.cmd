<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetVisible visible="true"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.5" g="0" b="0"/>
    </scriptStep>
  </command>
  
  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
    </scriptStep>
  </command>

  <command name="transitionIn" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor a="0"/>
      <ColorToDefault method="time" time="250" a=""/>
    </scriptStep>
  </command>

  <command name="transitionOut" endMode="stop">
    <scriptStep>
      <SetColorToDefault a=""/>
      <ColorTo method="time" time="250" a="0"/>
    </scriptStep>
  </command>
  
</commandScripts>
