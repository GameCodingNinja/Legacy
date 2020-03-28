<commandScripts>

  <command name="idleForward" endMode="stop">
    <scriptStep>
      <SetFrame frame="6"/>
    </scriptStep>
  </command>

  <command name="idleBackward" endMode="stop">
    <scriptStep>
      <SetFrame frame="18"/>
    </scriptStep>
  </command>

  <command name="idleLeft" endMode="stop">
    <scriptStep>
      <SetFrame frame="30"/>
    </scriptStep>
  </command>

  <command name="idleRight" endMode="stop">
    <scriptStep>
      <SetFrame frame="42"/>
    </scriptStep>
  </command>
  
  
  <command name="runForward" endMode="loop">
    <scriptStep>
      <AnimateFrame frameStart="0" frameEnd="11" fps="20"/>
    </scriptStep>
  </command>

  <command name="runBackward" endMode="loop">
    <scriptStep>
      <AnimateFrame frameStart="12" frameEnd="23" fps="20"/>
    </scriptStep>
  </command>

  <command name="runLeft" endMode="loop">
    <scriptStep>
      <AnimateFrame frameStart="24" frameEnd="35" fps="20"/>
    </scriptStep>
  </command>

  <command name="runRight" endMode="loop">
    <scriptStep>
      <AnimateFrame frameStart="36" frameEnd="47" fps="20"/>
    </scriptStep>
  </command>
  
</commandScripts>
