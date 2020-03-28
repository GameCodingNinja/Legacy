<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.15" g="0.29" b="0.46"/>
    </scriptStep>

    <scriptStep>
      <SetHold hold="50"/>
    </scriptStep>

    <scriptStep>
      <SetColor r="0" g="0" b="0"/>
    </scriptStep>
  </command>
  
  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>


  <command name="transitionIn" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>

  <command name="transitionOut" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>
  
</commandScripts>
