using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Threading;

namespace AStarPathfinding
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Game1 : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        private Graph graph;
        private Dictionary<string, Texture2D> textures;
        private float delay;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            graphics.PreferredBackBufferWidth = 500;
            graphics.PreferredBackBufferHeight = 500;
            graphics.ApplyChanges();
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            Vertex start = new Vertex(2 * Vertex.TileSize, 1 * Vertex.TileSize);
            start.Texture = "start";            
            Vertex end = new Vertex(6 * Vertex.TileSize, 7 * Vertex.TileSize);
            end.Texture = "end";
            graph = new Graph(start, end);
            textures = new Dictionary<string, Texture2D>();
            delay = 0f;

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // TODO: use this.Content to load your game content here
            textures.Add("grid", Content.Load<Texture2D>("gridSquare"));
            textures.Add("path", Content.Load<Texture2D>("gridSquarePath"));
            textures.Add("start", Content.Load<Texture2D>("gridSquareStart"));
            textures.Add("end", Content.Load<Texture2D>("gridSquareEnd"));
            textures.Add("open", Content.Load<Texture2D>("gridSquareOpenList"));
            textures.Add("closed", Content.Load<Texture2D>("gridSquareClosedList"));
            textures.Add("obstacle", Content.Load<Texture2D>("gridObstacle"));
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// game-specific content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Add your update logic here
            // Attempt at a timer
            delay += (float)gameTime.ElapsedGameTime.TotalSeconds;

            if (delay > .5f)
            {
                delay = 0;
                graph.AStarAlgorithm();
            }           

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Add your drawing code here
            spriteBatch.Begin();

            foreach (Vertex v in graph.Vertices)
            {
                if(v.Texture == null)
                {
                    v.Texture = "grid";
                }

                spriteBatch.Draw(textures[v.Texture], v.Rect, Color.White);
            }

            spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
